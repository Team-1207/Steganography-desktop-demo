module ru.shemplo.dss {
    requires lombok;
    requires org.slf4j;
    
    requires transitive javafx.controls;
    requires transitive javafx.graphics;
    requires transitive javafx.fxml;
    requires java.desktop;
    requires tiwulfx;
    
    //requires org.apache.commons.imaging;
    
    exports ru.shemplo.sdd;
}