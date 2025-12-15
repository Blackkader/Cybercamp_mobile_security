package io.securinets.level2;

public class heavynativeflag {
    static { System.loadLibrary("encflag"); }
    public native byte[] magic(String input);
}
