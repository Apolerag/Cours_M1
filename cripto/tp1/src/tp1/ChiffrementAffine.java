/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tp1;

import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

/**
 *
 * @author apolerag
 */
public final class ChiffrementAffine extends Cripto {
//texte 1 9x+19
// texte 2 19x+3

    final int[] clef = new int[]{1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
    int[] trad = new int[26];

    public ChiffrementAffine(String fic) throws FileNotFoundException, IOException {
        super(fic);
        decriptage();
    }

    public ChiffrementAffine() throws FileNotFoundException, IOException {
        super();
        decriptage();
    }

    void decriptage() throws IOException {
        PrintWriter ecri;
        try {
            ecri = new PrintWriter(new FileWriter("sortie.txt"));
            char[] c = s.toCharArray();
            int i, b, k, x;
            int comp = 1;
            for (i = 0; i < clef.length; i++) {
                for (b = 0; b < 26; b++) {
                    //Pour chaque lettre on rempli le tableau trad avec les valeurs de chaque lettre décriptée
                    for (x = 0; x < 26; x++) {

                        int l = Math.abs((x + b) * clef[i]);

                        l = l % 26;
                        trad[x] = l;
                    }
                    c = s.toCharArray();
                    for (k = 0; k < c.length; k++) {
                        c[k] = (char) (trad[c[k] - 'a'] + 'a');
                    }
                    ecri.print(comp + " a = " + clef[i] + " b = " + b + " ");
                    comp++;
                    ecri.print(c);
                    ecri.print("\n\n");


                }

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
