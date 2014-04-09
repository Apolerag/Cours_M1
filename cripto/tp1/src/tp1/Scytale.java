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
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public final class Scytale extends Cripto {

    public Scytale(String fic) throws FileNotFoundException {
        super(fic);
        decriptage();
    }

    public Scytale() throws FileNotFoundException {
        super();
        decriptage();
    }

    void decriptage() {
        PrintWriter ecri;
        try {
            ecri = new PrintWriter(new FileWriter("sortie.txt"));
            char[] c = s.toCharArray();
            char[] r = new char[c.length];
            int pas = 1;
            for (pas = 1; pas < c.length / 2; pas++) {
                int i = 0;
                int j = 0;
                int d = 0;
                while (i < c.length) {
                    System.out.println("i = " + i + " j = " + j + " d = " + d);
                    r[j] = c[i];
                    i++;
                    j += pas;
                    if (j >= c.length) {
                        d++;
                        j = d;
                    }
                }
                ecri.print(pas + " ");
                ecri.print(r);
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

    void compteOccu(char[] c) {
        int[] occurence = new int[26];
        for (int i = 0; i < c.length; i++) {
            int lettre = c[i] - 'a';
            occurence[lettre]++;
        }
        for (int i = 0; i < 26; i++) {
            char k = (char) ('a' + i);
            System.out.print(k + "=");
            System.out.print(occurence[i] + " ");
        }
        System.out.println();
    }
}
