package com.vpavlov.pockerclient.connection;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Locale;

public class Connection {

    public static final Logger LOGGER = LogManager.getLogger();

    private final String serverName;

    private final Integer serverPort;

    public Connection(String serverName, Integer serverPort) {
        this.serverName = serverName;
        this.serverPort = serverPort;
    }
    public Connection(Integer serverPort) {
        this(null, serverPort);
    }

    public Socket connect() throws IOException {

        Socket socket = null;
        if (serverName == null){
            LOGGER.debug(String.format(Locale.US,"Connecting to the: '%s:%d'", InetAddress.getLocalHost(), serverPort));
            socket = new Socket(InetAddress.getLocalHost(), serverPort);
        }else{
            LOGGER.debug(String.format(Locale.US,"Connecting to the: '%s:%d'", serverName, serverPort));
            socket = new Socket(InetAddress.getByName(serverName), serverPort);
        }
        LOGGER.debug("Connection successful.");
        return socket;
    }




}
