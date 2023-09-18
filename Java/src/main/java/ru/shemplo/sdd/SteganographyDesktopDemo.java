package ru.shemplo.sdd;

import java.io.File;
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
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.FileChooser.ExtensionFilter;
import javafx.stage.Stage;
import javafx.stage.Window;

public class SteganographyDesktopDemo extends Application {

    public static final File DOWNLOADS_DIR = System.getProperty ("os.name").contains ("Win")
        ? new File ("C:\\Users\\" + System.getProperty ("user.name") + "\\Downloads")
        : new File ("/home/" + System.getProperty ("user.name") + "/downloads");
    
    @Override
    public void start (Stage stage) throws Exception {
        final var scene = new Scene (makeScene (stage.getOwner ()));
        
        stage.setTitle ("Стеганограф (только чтение)");
        stage.getIcons ().add (UIIcons.mail64);
        stage.setScene (scene);
        stage.show ();
        
        stage.sizeToScene ();
    }
    
    private Parent makeScene (Window window) {
        final var column = new VBox (8.0);
        column.setPadding (new Insets (8.0));
        
        final var imagePreview = new ImageView ();
        column.getChildren ().add (imagePreview);
        imagePreview.setFitWidth (800.0);
        imagePreview.setFitHeight (imagePreview.getFitWidth () / 16.0 * 9.0);
        
        final var chooseRow = new HBox (8.0);
        column.getChildren ().add (chooseRow);
        chooseRow.setAlignment (Pos.BASELINE_LEFT);
        
        final var chooseButton = new Button ("Выбрать изображение");
        chooseRow.getChildren ().add (chooseButton);
        
        chooseRow.getChildren ().add (new Text ("Путь к текущему изображению: "));
        
        final var filepathText = new Text ("(изображение не выбрано)");
        chooseRow.getChildren ().add (filepathText);        
        
        final var previewRow = new HBox (8.0);
        column.getChildren ().add (previewRow);
        
        final var previewText = new TextArea ();
        previewRow.getChildren ().add (previewText);
        previewText.setMaxHeight (75.0);
        
        final var exifColumn = new VBox (8.0);
        previewRow.getChildren ().add (exifColumn);
        
        exifColumn.getChildren ().add (new Text ("EXIF metadata"));
        
        final var exifDescription = new Text ();
        exifColumn.getChildren ().add (exifDescription);
        
        final var exifCopyright = new Text ();
        exifColumn.getChildren ().add (exifCopyright);
        
        final var previewRawRow = new HBox (8.0);
        column.getChildren ().add (previewRawRow);
        
        final var previewRawText = new TextArea ();
        previewRawRow.getChildren ().add (previewRawText);
        HBox.setHgrow (previewRawText, Priority.ALWAYS);
        
        chooseButton.setOnMouseClicked (me -> {
            final var chooser = new FileChooser ();
                        
            final var mainFilter = new ExtensionFilter ("Images", "*.png", "*.jpg", "*.bmp");
            chooser.getExtensionFilters ().add (mainFilter);
            chooser.getExtensionFilters ().add (new ExtensionFilter ("All files", "*.*"));
            chooser.setSelectedExtensionFilter (mainFilter);
            chooser.setInitialDirectory (DOWNLOADS_DIR);
            
            final var file = chooser.showOpenDialog (window);
            if (file != null && file.canRead ()) {                
                try (final var is = new FileInputStream (file)) {
                    final var image = new Image (is);
                    
                    previewText.setText (SteganographyEngine.getInstance ().decode (image));
                    filepathText.setText (file.getAbsolutePath ());
                    imagePreview.setImage (image);
                } catch (IOException ioe) {
                    ioe.printStackTrace ();
                }
                
                try (final var is = new FileInputStream (file)) {
                    final var bytes = is.readNBytes (20000);
                    int start = 79, i = start;
                    while (bytes [i++] != 0) {}
                    exifDescription.setText ("Description: " + new String (bytes, start, i - start));
                    
                    start = i;
                    while (bytes [i++] != 0) {}
                    exifCopyright.setText ("Copyright: " + new String (bytes, start, i - start));
                    
                    previewRawText.setText (new String (bytes));
                } catch (IOException ioe) {
                    ioe.printStackTrace ();
                }
            }
        });
        
        return column;
    }
    
}
