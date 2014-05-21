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

    private BigInteger p;
    private BigInteger q;
    private BigInteger N;
    private BigInteger e;
    private BigInteger d;
    private BigInteger FiN;

    public RSA(int nbBits, int certainty, Random p) {
        //p et q premier
        this.p = getPrime(nbBits, certainty, p);
        this.q = getPrime(nbBits, certainty, p);
        //FiN = (p-1)(q-1)
        this.FiN = (this.p.subtract(BigInteger.ONE)).multiply(this.q.subtract(BigInteger.ONE));
        //N = p*q
        this.N = this.p.multiply(this.q);
       do {
            e = new BigInteger(FiN.bitLength(), new Random());
            e = e.mod(FiN);
        } while(e.intValue() == 0);
        while((BigInteger.ONE).compareTo( FiN.gcd(e))  != 0){
            e = e.add(BigInteger.ONE);
        }
        BigInteger d = e.modInverse(FiN);
        // e*d.mod(Fin) == 1
        this.d = e.modInverse(FiN);
        System.out.println("d " + d);
        System.out.println("e " + e);
        System.out.println("d " + d);
        System.out.println("d%e " + e.mod(d));
        System.out.println("(e.multiply(d)).mod(FiN) " + (e.multiply(d)).mod(FiN));
        System.out.println("n " + N);

    }

    private BigInteger getPrime(int nbBits, int certainty, Random prg) {
        return new BigInteger(nbBits, certainty, prg);
    }

    public BigInteger chiffre(String message) {
        System.out.println("chiffre() :");
        BigInteger mess_crypte = new BigInteger(message);

        System.out.println("mess_crypte :" + mess_crypte);
        mess_crypte = mess_crypte.modPow(e, N);
        System.out.println("mess_crypte :" + mess_crypte);
        
        System.out.println("Fin chiffre() :");
        return mess_crypte;
    }

    public String dechiffre(BigInteger mess_crypte) {
        System.out.println("dechiffre() :");
        
        System.out.println("mess_crypte :" + mess_crypte);
        mess_crypte = mess_crypte.modPow(d, N);
        
        System.out.println("mess_crypte :" + mess_crypte);
        
        System.out.println("Fin dechiffre() :");
        return mess_crypte.toString();
    }

    public BigInteger getFiN() {
        return FiN;
    }

    public void setFiN(BigInteger FiN) {
        this.FiN = FiN;
    }

    public BigInteger getN() {
        return N;
    }

    public void setN(BigInteger N) {
        this.N = N;
    }

    public BigInteger getD() {
        return d;
    }

    public void setD(BigInteger d) {
        this.d = d;
    }

    public BigInteger getE() {
        return e;
    }

    public void setE(BigInteger e) {
        this.e = e;
    }

    public BigInteger getP() {
        return p;
    }

    public void setP(BigInteger p) {
        this.p = p;
    }

    public BigInteger getQ() {
        return q;
    }

    public void setQ(BigInteger q) {
        this.q = q;
    }
}
