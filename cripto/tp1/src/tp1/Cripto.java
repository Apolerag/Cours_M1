/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tp1;

/**
 *
 * @author apolerag
 */
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.FileReader;

public class Cripto {

    String s;
    String nomFichier;

    public Cripto(String fic) throws FileNotFoundException {
        this.nomFichier = fic;
        Scanner scanner;
        scanner = new Scanner(new FileReader(nomFichier));
        s = scanner.nextLine();
        scanner.close();
    }
    
    public Cripto() throws FileNotFoundException {
        this.nomFichier = "texte.txt";
        Scanner scanner;
        scanner = new Scanner(new FileReader(nomFichier));
        s = scanner.nextLine();
        scanner.close();
    }

}
