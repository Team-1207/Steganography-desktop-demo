package ru.shemplo.sdd;
import java.io.IOException;
import java.util.Optional;

import javafx.scene.image.Image;

public class UIIcons {
    
    public static final Image mail64 = loadIcon ("image-mail.64.png");
    public static final Image mail256 = loadIcon ("image-mail.256.png");
    
    private static Image loadIcon (String name) {
        final var path = String.format ("/%s/%s", "icons", name);
        try (final var is = UIIcons.class.getResourceAsStream (path)) {            
            return Optional.ofNullable (is).map (Image::new).orElse (null);
        } catch (IOException ioe) {
            ioe.printStackTrace ();
            return null;
        }
    }
    
}