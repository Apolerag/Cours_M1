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
        RSA r = new RSA(128, 0, new Random());

        int p, q, d, e, N, FiN;
        p = 103;
        q = 109;
        N = p * q;
        FiN = (p - 1) * (q - 1);
        e = p % (FiN - 1) + 1;
        
        System.out.println(e);

    }
}
