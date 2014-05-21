/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package chiffrement.rsa;

import java.math.BigInteger;

/**
 *
 * @author apolerag
 */
public class PrivateKey {

    private BigInteger p;
    private BigInteger q;
    
    private BigInteger d;

    public PrivateKey(BigInteger p, BigInteger q, BigInteger d) {
        this.p = p;
        this.q = q;
        this.d = d;
    }
    
    
}
