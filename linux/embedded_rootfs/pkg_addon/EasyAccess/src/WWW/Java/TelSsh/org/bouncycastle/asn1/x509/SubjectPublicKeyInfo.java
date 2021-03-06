package org.bouncycastle.asn1.x509;

import java.io.*;
import java.util.Enumeration;
import java.math.BigInteger;

import org.bouncycastle.asn1.*;
import org.bouncycastle.asn1.pkcs.PKCSObjectIdentifiers;

/**
 * The object that contains the public key stored in a certficate.
 * <p>
 * The getEncoded() method in the public keys in the JCE produces a DER
 * encoded one of these.
 */
public class SubjectPublicKeyInfo
    implements DEREncodable
{
    private AlgorithmIdentifier     algId;
    private DERBitString            keyData;

    public static SubjectPublicKeyInfo getInstance(
        ASN1TaggedObject obj,
        boolean          explicit)
    {
        return getInstance(ASN1Sequence.getInstance(obj, explicit));
    }

    public static SubjectPublicKeyInfo getInstance(
        Object  obj)
    {
        if (obj instanceof SubjectPublicKeyInfo)
        {
            return (SubjectPublicKeyInfo)obj;
        }
        else if (obj instanceof ASN1Sequence)
        {
            return new SubjectPublicKeyInfo((ASN1Sequence)obj);
        }

        throw new IllegalArgumentException("unknown object in factory");
    }

    public SubjectPublicKeyInfo(
        AlgorithmIdentifier algId,
        DEREncodable        publicKey)
    {
        this.keyData = new DERBitString(publicKey);
        this.algId = algId;
    }

    public SubjectPublicKeyInfo(
        AlgorithmIdentifier algId,
        byte[]              publicKey)
    {
        this.keyData = new DERBitString(publicKey);
        this.algId = algId;
    }

    public SubjectPublicKeyInfo(
        ASN1Sequence  seq)
    {
        Enumeration         e = seq.getObjects();

        this.algId = AlgorithmIdentifier.getInstance(e.nextElement());
        this.keyData = (DERBitString)e.nextElement();
    }

    public AlgorithmIdentifier getAlgorithmId()
    {
        return algId;
    }

    /**
     * for when the public key is an encoded object - if the bitstring
     * can't be decoded this routine throws an IOException.
     *
     * @exception IOException - if the bit string doesn't represent a DER
     * encoded object.
     */
    public DERObject getPublicKey()
        throws IOException
    {
        ByteArrayInputStream    bIn = new ByteArrayInputStream(keyData.getBytes());
        DERInputStream          dIn = new DERInputStream(bIn);

        return dIn.readObject();
    }

    /**
     * for when the public key is raw bits...
     */
    public DERBitString getPublicKeyData()
    {
        return keyData;
    }

    /**
     * <pre>
     * SubjectPublicKeyInfo ::= SEQUENCE {
     *                          algorithm AlgorithmIdentifier,
     *                          publicKey BIT STRING }
     * </pre>
     */
    public DERObject getDERObject()
    {
        DERConstructedSequence  seq = new DERConstructedSequence();

        seq.addObject(algId);
        seq.addObject(keyData);

        return seq;
    }
}
