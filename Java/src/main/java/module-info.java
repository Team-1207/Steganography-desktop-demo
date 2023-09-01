module ru.shemplo.dss {
    requires lombok;
    requires org.slf4j;
    
    requires transitive javafx.graphics;
    requires transitive javafx.controls;
    requires transitive javafx.fxml;
    requires tiwulfx;
    
    exports ru.shemplo.sdd;
}