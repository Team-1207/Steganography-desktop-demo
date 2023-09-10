package ru.shemplo.sdd.input;

public class RunInputListener {
    
    public static void main (String ... args) {
        final var consoleListener = new Thread (new InputConsoleListener ());
        consoleListener.setName ("input - console listener");
        consoleListener.start ();
        
        final var socketListener = new Thread (new InputSocketListener ());
        socketListener.setName ("input - socket listener");
        socketListener.setDaemon (true);
        socketListener.start ();
    }
    
}
