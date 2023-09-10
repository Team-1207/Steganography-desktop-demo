package ru.shemplo.sdd.input;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.nio.charset.StandardCharsets;

import lombok.extern.slf4j.Slf4j;
import ru.shemplo.sdd.SteganographyDesktopDemo;

@Slf4j
public class InputSocketListener implements Runnable {

    private final File SAVE_DIR = new File (SteganographyDesktopDemo.DOWNLOADS_DIR, "Steganography");
    
    @Override
    public void run () {
        try (final var serverSocket = new ServerSocket (1207)) {
            log.info ("Socket listener is initialized");
            
            if (!SAVE_DIR.exists ()) {
                log.info ("Creating folder for input files...");
                SAVE_DIR.mkdir ();
                
                log.info ("Creating folder for input files... DONE");
            }
            
            while (true) {
                try (
                    final var socket = serverSocket.accept (); 
                    final var is = socket.getInputStream ();
                ) {
                    log.info ("Connection is established");
                    
                    final var filename = new String (is.readNBytes (is.read ()), StandardCharsets.UTF_8);
                    final var file = new File (SAVE_DIR, filename);
                    
                    log.info ("Trying to save `{}` file to `{}`...", filename, file.getAbsolutePath ());
                    
                    try (final var os = new FileOutputStream (file)) {
                        is.transferTo (os);
                    }
                    
                    log.info ("File saved, connection closed");
                } catch (IOException ioe) {
                    ioe.printStackTrace ();
                }
            }
        } catch (IOException ioe) {
            ioe.printStackTrace ();
        }
    }
    
}
