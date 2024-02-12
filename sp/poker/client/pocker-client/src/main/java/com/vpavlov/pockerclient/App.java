package com.vpavlov.pockerclient;


import com.vpavlov.pockerclient.connection.Connection;
import com.vpavlov.pockerclient.connection.Constants;
import com.vpavlov.pockerclient.connection.Message;
import com.vpavlov.pockerclient.connection.Status;
import com.vpavlov.pockerclient.connection.header.Header;
import com.vpavlov.pockerclient.connection.payload.Payload;
import com.vpavlov.pockerclient.connection.type.Subtype;
import com.vpavlov.pockerclient.connection.type.Type;
import com.vpavlov.pockerclient.ui.cli.CLI;
import com.vpavlov.pockerclient.ui.i18n.I18n;
import com.vpavlov.pockerclient.ui.javafx.GUI;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;
import java.util.Locale;
import java.util.Properties;

public class App{

    private static final String USE_GUI_PARAM = "gui";

    static {
        System.setProperty("log4j.configurationFile",
                App.class.getResource("config/log4j-config.xml").getPath());
    }

    private static final Logger LOGGER = LogManager.getLogger(App.class);

    private static I18n i18n;

    private static void loadProps(){
        i18n = I18n.load(Locale.US);
    }

    public static void main(String[] args) {
        loadProps();
        LOGGER.debug(Locale.US.toString());
        Properties prop = new Properties();
        for (String arg: args){
            if (USE_GUI_PARAM.equals(arg.toLowerCase())){
                GUI.launch();
                break;
            }
        }
        CLI.launch(args);

        Connection conn = new Connection("localhost", 10000);
        Payload payload = new Payload();
        payload.setValue("test", 1234);
        Header header = new Header(Type.POST, Subtype.PING);
        header.setStatus(Status.NO_STATUS);
        header.setIdentifier(Constants.IDENTIFIER);
        try {
            conn.connect();
            while(true){
                conn.sendMessage(new Message(header, payload));
                Thread.sleep(5000);
            }
        } catch (Exception e) {
            LOGGER.error(e);
        }
    }

    public static I18n getI18n(){
        return i18n;
    }
}