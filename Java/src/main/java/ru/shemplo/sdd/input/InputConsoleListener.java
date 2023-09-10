package ru.shemplo.sdd.input;

import java.util.Scanner;

import lombok.extern.slf4j.Slf4j;

@Slf4j
public class InputConsoleListener implements Runnable {

    @Override
    public void run () {
        try (final var scanner = new Scanner (System.in)) {
            log.info ("Console listener is initialized");
            while (true) {
                final var command = scanner.next ();
                if ("stop".equals (command)) {
                    log.info ("Stop is called. Stopping listeners...");
                    break;
                }
            }
        }
    }
    
}
