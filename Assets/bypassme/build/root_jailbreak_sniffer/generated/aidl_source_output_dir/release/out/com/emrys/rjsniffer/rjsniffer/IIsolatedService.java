/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Using: /home/blackkader/Android/Sdk/build-tools/35.0.0/aidl -p/home/blackkader/Android/Sdk/platforms/android-34/framework.aidl -o/home/blackkader/All/CyberCamp/Assets/bypassme/build/root_jailbreak_sniffer/generated/aidl_source_output_dir/release/out -I/home/blackkader/.pub-cache/hosted/pub.dev/root_jailbreak_sniffer-1.1.4/android/src/main/aidl -I/home/blackkader/.pub-cache/hosted/pub.dev/root_jailbreak_sniffer-1.1.4/android/src/release/aidl -I/home/blackkader/.gradle/caches/8.14/transforms/c6084a3565a35fe75bf53a54a41df9ae/transformed/core-1.13.1/aidl -I/home/blackkader/.gradle/caches/8.14/transforms/df273792364e947516efc073187ee66b/transformed/versionedparcelable-1.1.1/aidl -d/tmp/aidl4097663804120328748.d /home/blackkader/.pub-cache/hosted/pub.dev/root_jailbreak_sniffer-1.1.4/android/src/main/aidl/com/emrys/rjsniffer/rjsniffer/IIsolatedService.aidl
 */
package com.emrys.rjsniffer.rjsniffer;
// Declare any non-default types here with import statements
public interface IIsolatedService extends android.os.IInterface
{
  /** Default implementation for IIsolatedService. */
  public static class Default implements com.emrys.rjsniffer.rjsniffer.IIsolatedService
  {
    /**
     * Demonstrates some basic types that you can use as parameters
     * and return values in AIDL.
     */
    @Override public boolean isMagiskPresent() throws android.os.RemoteException
    {
      return false;
    }
    @Override
    public android.os.IBinder asBinder() {
      return null;
    }
  }
  /** Local-side IPC implementation stub class. */
  public static abstract class Stub extends android.os.Binder implements com.emrys.rjsniffer.rjsniffer.IIsolatedService
  {
    /** Construct the stub at attach it to the interface. */
    @SuppressWarnings("this-escape")
    public Stub()
    {
      this.attachInterface(this, DESCRIPTOR);
    }
    /**
     * Cast an IBinder object into an com.emrys.rjsniffer.rjsniffer.IIsolatedService interface,
     * generating a proxy if needed.
     */
    public static com.emrys.rjsniffer.rjsniffer.IIsolatedService asInterface(android.os.IBinder obj)
    {
      if ((obj==null)) {
        return null;
      }
      android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
      if (((iin!=null)&&(iin instanceof com.emrys.rjsniffer.rjsniffer.IIsolatedService))) {
        return ((com.emrys.rjsniffer.rjsniffer.IIsolatedService)iin);
      }
      return new com.emrys.rjsniffer.rjsniffer.IIsolatedService.Stub.Proxy(obj);
    }
    @Override public android.os.IBinder asBinder()
    {
      return this;
    }
    @Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
    {
      java.lang.String descriptor = DESCRIPTOR;
      if (code >= android.os.IBinder.FIRST_CALL_TRANSACTION && code <= android.os.IBinder.LAST_CALL_TRANSACTION) {
        data.enforceInterface(descriptor);
      }
      if (code == INTERFACE_TRANSACTION) {
        reply.writeString(descriptor);
        return true;
      }
      switch (code)
      {
        case TRANSACTION_isMagiskPresent:
        {
          boolean _result = this.isMagiskPresent();
          reply.writeNoException();
          reply.writeInt(((_result)?(1):(0)));
          break;
        }
        default:
        {
          return super.onTransact(code, data, reply, flags);
        }
      }
      return true;
    }
    private static class Proxy implements com.emrys.rjsniffer.rjsniffer.IIsolatedService
    {
      private android.os.IBinder mRemote;
      Proxy(android.os.IBinder remote)
      {
        mRemote = remote;
      }
      @Override public android.os.IBinder asBinder()
      {
        return mRemote;
      }
      public java.lang.String getInterfaceDescriptor()
      {
        return DESCRIPTOR;
      }
      /**
       * Demonstrates some basic types that you can use as parameters
       * and return values in AIDL.
       */
      @Override public boolean isMagiskPresent() throws android.os.RemoteException
      {
        android.os.Parcel _data = android.os.Parcel.obtain();
        android.os.Parcel _reply = android.os.Parcel.obtain();
        boolean _result;
        try {
          _data.writeInterfaceToken(DESCRIPTOR);
          boolean _status = mRemote.transact(Stub.TRANSACTION_isMagiskPresent, _data, _reply, 0);
          _reply.readException();
          _result = (0!=_reply.readInt());
        }
        finally {
          _reply.recycle();
          _data.recycle();
        }
        return _result;
      }
    }
    static final int TRANSACTION_isMagiskPresent = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
  }
  /** @hide */
  public static final java.lang.String DESCRIPTOR = "com.emrys.rjsniffer.rjsniffer.IIsolatedService";
  /**
   * Demonstrates some basic types that you can use as parameters
   * and return values in AIDL.
   */
  public boolean isMagiskPresent() throws android.os.RemoteException;
}
