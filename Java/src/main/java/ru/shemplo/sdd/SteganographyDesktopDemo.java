package ru.shemplo.sdd;

import java.io.FileInputStream;
import java.io.IOException;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;
import javafx.stage.Stage;
import javafx.stage.Window;

public class SteganographyDesktopDemo extends Application {

    @Override
    public void start (Stage stage) throws Exception {
        final var scene = new Scene (makeScene (stage.getOwner ()));
        
        stage.getIcons ().add (UIIcons.mail64);
        stage.setTitle ("Стеганограф");
        stage.setScene (scene);
        stage.show ();
        
        stage.sizeToScene ();
    }
    
    private Parent makeScene (Window window) {
        final var column = new VBox (8.0);
        column.setPadding (new Insets (8.0));
        column.setMinWidth (800.0);
        
        final var imagePreview = new ImageView ();
        column.getChildren ().add (imagePreview);
        imagePreview.setFitWidth (700.0);
        imagePreview.setFitHeight (700.0 / 16.0 * 9.0);
        
        final var chooseRow = new HBox (8.0);
        column.getChildren ().add (chooseRow);
        chooseRow.setAlignment (Pos.CENTER);
        
        final var chooseButton = new Button ("Выбрать изображение");
        chooseRow.getChildren ().add (chooseButton);
        
        final var previewText = new TextArea ();
        column.getChildren ().add (previewText);
        
        chooseButton.setOnMouseClicked (me -> {
            final var chooser = new FileChooser ();
            
            final var mainFilter = new ExtensionFilter ("Image", "*.png", "*.jpg");
            chooser.getExtensionFilters ().add (mainFilter);
            chooser.setSelectedExtensionFilter (mainFilter);
            
            final var file = chooser.showOpenDialog (window);
            if (file != null && file.canRead ()) {                
                try (final var is = new FileInputStream (file)) {
                    final var image = new Image (is);
                    
                    imagePreview.setImage (image);
                    
                    final var text = SteganographyEngine.getInstance ().decode (image);
                    previewText.setText (text);
                } catch (IOException ioe) {
                    ioe.printStackTrace ();
                }
            }
        });
        
        return column;
    }
    
}
