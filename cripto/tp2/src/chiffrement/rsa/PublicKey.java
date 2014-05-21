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
public class PublicKey {
    private BigInteger N;
    private BigInteger e;

    public PublicKey(BigInteger N, BigInteger e) {
        this.N = N;
        this.e = e;
    }
    
    
}
