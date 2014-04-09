/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tp1;

import java.io.FileNotFoundException;
import java.io.IOException;

/**
 *
 * @author apolerag
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws FileNotFoundException, IOException {
        ChiffrementAffine c = new ChiffrementAffine("texte.txt");

//        int[] clef = new int[]{1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
//        int[] trad = new int[26];
//
//        for (int b = 0; b < 26; b++) {
//            int a = clef[3];
//            for (int x = 0; x < 26; x++) {
//                int l = Math.abs((x + b) * a);
//
//                l = l % 26;
//                trad[x] = l;
//            }
//            for (int k = 0; k < 26; k++) {
//                System.out.print((char) (trad[k] + 'a') + " ");
//            }
//            System.out.println();
//        }
    }
}
