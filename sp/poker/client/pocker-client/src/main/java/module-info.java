module com.vpavlov.pockerclient {
    requires javafx.controls;
    requires javafx.fxml;
    requires javafx.web;

    requires org.controlsfx.controls;
    requires com.dlsc.formsfx;
    requires org.kordamp.bootstrapfx.core;
    requires org.apache.logging.log4j;

    opens com.vpavlov.pockerclient to javafx.fxml;
    exports com.vpavlov.pockerclient;
}