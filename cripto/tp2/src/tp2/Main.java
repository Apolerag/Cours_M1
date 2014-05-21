/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package tp2;

import chiffrement.rsa.RSA;
import java.math.BigInteger;
import java.util.Random;

/**
 *
 * @author apolerag
 */
public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        RSA r = new RSA(1024, 30, new Random());
        String message = "123456789";
        System.out.println("Le message est : " + message);
        BigInteger message_crypte = r.chiffre(message);
        String s = r.dechiffre(message_crypte);
        System.out.println("Le message cripté est : " + message_crypte);
        System.out.println("Le message décripté est : " + s);

    }
}
