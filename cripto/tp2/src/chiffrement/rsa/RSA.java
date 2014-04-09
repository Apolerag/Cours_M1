/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package chiffrement.rsa;

import java.math.BigInteger;
import java.util.Random;

/**
 *
 * @author apolerag
 */
public class RSA {

    BigInteger p;
    BigInteger q;
    BigInteger N;
    BigInteger e;
    BigInteger d;
    BigInteger un;
    BigInteger FiN;

    public RSA(int nbBits, int certainty, Random p) {
        this.un = new BigInteger("1");
        //p et q premier
        this.p = getPrime(nbBits, certainty, p);
        this.q = getPrime(nbBits, certainty, p);
        //FiN = (p-1)(q-1)
        this.FiN = (this.p.subtract(un)).multiply(this.q.subtract(un));
        //N = p*q
        this.N = this.p.multiply(this.q);
        this.e = new BigInteger(nbBits, p).mod(FiN.subtract(un)).add(un);
        //e doit être premier avec FiN
        while (this.e.gcd(FiN).equals(un) && this.e.compareTo(FiN) == -1) {
            this.e = new BigInteger(nbBits, p).mod(FiN.subtract(un)).add(un);
        }
        // e*d.mod(Fin) == 1
        this.d = new BigInteger(nbBits, p);
        BigInteger m = (d.multiply(e)).mod(FiN);
        while (!m.equals(un)) {
            this.d = this.d.subtract(m).add(un);
            m = (d.multiply(e)).mod(FiN);
            System.out.println("d " + d);
            System.out.println("(d.multiply(e)).mod(FiN) " + (d.multiply(e)).mod(FiN));
            System.out.println("(d.multiply(e)).mod(FiN).equals(un) " + (d.multiply(e)).mod(FiN).equals(un));
        }
        BigInteger t = e.mod(d);
        System.out.println("d " + d);
        System.out.println("e " + e);
        System.out.println("t " + t);
        System.out.println("d-t " + d.subtract(t));
        d = d.subtract(t).add(un);
        System.out.println("d " + d);
        System.out.println("d%e " + e.mod(d));

    }

    private BigInteger getPrime(int nbBits, int certainty, Random prg) {
        return new BigInteger(nbBits, certainty, prg);
    }
}
