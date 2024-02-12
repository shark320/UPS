package com.vpavlov.pockerclient.connection;

import com.vpavlov.pockerclient.connection.header.Header;
import com.vpavlov.pockerclient.connection.payload.Payload;
import com.vpavlov.pockerclient.connection.payload.mapping.PayloadParser;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Locale;
import java.util.concurrent.*;

public class Connection {

    public static final Logger LOGGER = LogManager.getLogger(Connection.class);

    private final String serverName;

    private final Integer serverPort;

    private final Thread receivingThread;

    private final Thread sendingThread;

    private final BlockingQueue<Message> receivedMessages = new LinkedBlockingQueue<>();
    private final BlockingQueue<Message> sendQueue = new LinkedBlockingQueue<>();

    private Socket socket;

    public Connection(String serverName, Integer serverPort) {
        this.serverName = serverName;
        this.serverPort = serverPort;

        receivingThread = new Thread(this::handleReceive, "ReceivingThread");

        sendingThread = new Thread(this::handleSend, "SendingThread");
    }

    public Connection(Integer serverPort) {
        this(null, serverPort);
    }

    public void connect() throws IOException {
        if (this.serverName == null) {
            LOGGER.debug(String.format(Locale.US, "Connecting to the: '%s:%d'", InetAddress.getLocalHost(), this.serverPort));
            this.socket = new Socket(InetAddress.getLocalHost(), this.serverPort);
        } else {
            LOGGER.debug(String.format(Locale.US, "Connecting to the: '%s:%d'", this.serverName, this.serverPort));
            this.socket = new Socket(InetAddress.getByName(this.serverName), this.serverPort);
        }
        LOGGER.debug("Connection successful.");
        this.receivingThread.start();
        this.sendingThread.start();
    }

    private void handleReceive() {
        LOGGER.debug("Starting receiving handler thread");
        try {
            DataInputStream input = new DataInputStream(this.socket.getInputStream());
            while (true) {
                Message receivedMessage = readMessage(input);
                LOGGER.trace("Received message: " + receivedMessage);
                this.receivedMessages.add(receivedMessage);
            }
        } catch (Exception e) {
            LOGGER.error("Server disconnected");
            //TODO: handle disconnection
        }
    }

    private Message readMessage(DataInputStream input) throws IOException {
        byte[] headerBytes = input.readNBytes(Constants.MSG_HEADER_LENGTH);
        String headerStr = new String(headerBytes);
        Header header = Header.extract(headerStr);
        int payloadLength = header.getLength();
        byte[] payloadBytes = input.readNBytes(payloadLength);
        String payloadStr = new String(payloadBytes);
        Payload payload = PayloadParser.parse(payloadStr);

        return new Message(header, payload);
    }

    private void handleSend() {
        LOGGER.debug("Starting sending handler thread");
        try {
            OutputStream output = this.socket.getOutputStream();
            while (true) {
                Message msg = this.sendQueue.take();
                String msgStr = msg.construct();
                LOGGER.trace("Sending message: " + msgStr);
                output.write(msgStr.getBytes());
                output.flush();
            }
        } catch (Exception e) {
            LOGGER.error("Server disconnected", e);
            //TODO: handle disconnection
        }
    }

    public void sendMessage(Message message) throws InterruptedException {
        this.sendQueue.put(message);
    }
}
