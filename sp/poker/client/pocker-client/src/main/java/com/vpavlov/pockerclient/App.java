package com.vpavlov.pockerclient;

import com.vpavlov.pockerclient.connection.Status;
import com.vpavlov.pockerclient.connection.header.Header;
import com.vpavlov.pockerclient.connection.payload.Payload;
import com.vpavlov.pockerclient.connection.payload.mapping.PayloadParser;
import com.vpavlov.pockerclient.connection.type.Subtype;
import com.vpavlov.pockerclient.connection.type.Type;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.IOException;

public class App extends Application {

    static {
        System.setProperty("log4j.configurationFile",
                App.class.getResource("config/log4j-config.xml").getPath());
    }

    private static Logger LOGGER = LogManager.getLogger();
    @Override
    public void start(Stage stage) throws IOException {
        FXMLLoader fxmlLoader = new FXMLLoader(App.class.getResource("hello-view.fxml"));
        Scene scene = new Scene(fxmlLoader.load(), 320, 240);
        stage.setTitle("Hello!");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        //launch();
        LOGGER.debug("test");

        String test_payload = "str=\"string\";int=12;int_arr=[1,2,3,4]";
        String test_response = String.format("%s%04d%1d%02d%03d", "POKR", test_payload.length(), Type.GET.getId(), Subtype.PING.getId(), Status.OK.getCode());
        //Payload payload = PayloadParser.parse(test_payload);
        System.out.println(Header.extract(test_response));
//        String str = "1243";
//
//        System.out.println("[" + str.substring(0,10) + "]");
    }
}