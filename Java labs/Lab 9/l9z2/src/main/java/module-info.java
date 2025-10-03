module com.example.l9z2 {
    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires org.kordamp.bootstrapfx.core;

    opens com.example.l9z2 to javafx.fxml;
    exports com.example.l9z2;
}