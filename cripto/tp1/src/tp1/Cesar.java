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
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public final class Cesar extends Cripto {

    public Cesar() throws FileNotFoundException {
        super();
        decriptage();
    }

    public Cesar(String fic) throws FileNotFoundException {
        super(fic);
        decriptage();
    }

    void decriptage() {
        PrintWriter ecri;
        try {
            ecri = new PrintWriter(new FileWriter("sortie.txt"));
            char[] c = s.toCharArray();
            compteOccu(c);
            // System.out.println(s);
            for (int i = 1; i <= 25; i++) {
                for (int j = 0; j < c.length; j++) {
                    c[j] = (char) (c[j] + 1);
                    if (c[j] > 'z') {
                        c[j] = 'a';
                    }
                }
                ecri.print(i + " ");
                ecri.print(c);
                ecri.print("\n\n");
            }

            ecri.flush();
            ecri.close();
        }//try
        catch (NullPointerException a) {
            System.out.println("Erreur : pointeur null");
        } catch (IOException a) {
            System.out.println("Problème d'IO");
        }

    }

    int[] compteOccu(char[] c) {
        int[] occurence = new int[26];
        for (int i = 0; i < c.length; i++) {
            int lettre = c[i] - 'a';
            occurence[lettre]++;
        }
        return occurence;
    }
}
