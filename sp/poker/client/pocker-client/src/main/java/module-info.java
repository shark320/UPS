module com.vpavlov.pockerclient {
    requires javafx.controls;
    requires javafx.fxml;
    requires javafx.web;

    requires org.controlsfx.controls;
    requires com.dlsc.formsfx;
    requires org.kordamp.bootstrapfx.core;
    requires eu.hansolo.tilesfx;

    opens com.vpavlov.pockerclient to javafx.fxml;
    exports com.vpavlov.pockerclient;
}