File: README
============

Table of Contents:
==================
01. What is in this release
02. Supported OCTEON_MODELs
03. Supported Operating Modes
04. Dependencies
05. Installed Directory Structure
06. Pre-requisites to Compiling the Engine
07. Building the Engine Library for OCTEON
08. Cleaning Engine Build
09. Testing Engine Using OpenSSL
10. Testing Engine Using Sample Application
11. Supported Features
12. Additional Documentation
13. Change History




01. What is in this release
===========================
This release contains the source code for the OCTEON OPENSSL CRYPTO ENGINE
v2.0 on OCTEON Linux.




02 Supported OCTEON_MODELs
==========================
    This release supports the following OCTEON_MODELs.

        - CN3xxx
        - CN5xxx
        - CN6xxx


    The following OCTEON_MODELs are not supported (deprecated) in this release.

        - OCTEON_CN38XX_PASS2
        - OCTEON_CN31XX_PASS1
        - OCTEON_CN31XX_PASS1_1
        - OCTEON_CN3020_PASS1
    



03. Supported Operating Modes
=============================
    This release supports the following Operating modes
        - Linux N32
        - Linux N64




04. Dependencies
================
This release requires the following OCTEON SDKs:

   a) OCTEON SDK v2.0.0

   b) OCTEON Linux v2.0.0




05. Installed Directory Structure
=================================

OCTEON SDK is usually installed under /usr/local/Cavium_Networks folder
as OCTEON-SDK.
This rpm is also installed under the same folder as

  |---OCTEON-SDK
      |
      |--- applications/linux_engine
      |         (source files for building the Engine)




06. Pre-requisites to Compiling the Engine 
==========================================
a) Setup the OCTEON development environment.

     # cd /usr/local/Cavium_Networks/OCTEON-SDK

     # source env-setup <OCTEON_MODEL> 

       Please refer SDK README.txt located at
       /usr/local/Cavium_Networks/OCTEON-SDK/ to setup the environment
       variables.
       This will setup the OCTEON_ROOT environment variable.

b) Configuration Selection

     # cd /usr/local/Cavium_Networks/OCTEON-SDK/linux/embedded_rootfs

     Select Toolchain ABI to be either N64 or N32 as follows,

     # make menuconfig

     Global Options  --->
        Toolchain ABI and C Library (N32 ABI with GNU C Library (glibc))  --->
                  (X) N32 ABI with GNU C library (glibc)
                  ( ) N64 ABI with GNU c library (glibc)

      Also make sure the following option is selected.

        [*] engine 

      Save this configuration and exit.

c) Building Octeon Linux Kernel

     # cd /usr/local/Cavium_Networks/OCTEON-SDK/linux
     
       If the OpenSSL Version is other than openssl-1.0.0a then 
       it is must to set the environmental variable OPENSSL_VERSION.
     
     # export OPENSSL_VERSION=x.x.xy 
              (for example: export OPENSSL_VERSION=1.0.0a)
     
     Before building OCTEON Linux kernel copy the openssl-1.0.0a.tar.gz
     in /usr/local/Cavium_Networks/OCTEON-SDK/linux/embedded_rootfs/storage directory.

     openssl-1.0.0a.tar.gz can be downloaded from the following URL.

          http://www.openssl.org/source/
     
     # make kernel
       It builds vmlinux.64 image and is available at 
        /usr/local/Cavium_Networks/OCTEON-SDK/linux/kernel_2.6/linux/


The below mentioned steps (d) and (e) must be executed after booting the 
vmlinux.64 image onto OCTEON.

Please refer to SDK documentation on HOWTO boot Linux on OCTEON.

d) Insert the cavium-ethernet module.
    
    # modprobe octeon-ethernet

e) Assign the IP address to one of the interface

    Ex:
    # ifconfig eth0 <IP>




07. Building the Engine Library for OCTEON
===========================================
   Compiling the Linux (step 5c) implicitly compiles the Engine for OCTEON,
   and the shared library is called "libocteon.so".

   The libocteon.so is available in the OCTEON Filesystem at
       /usr/lib64   if ABI N64 is selected
        or
       /usr/lib32   if ABI N32 is selected 




08. Cleaning Engine Build
==========================
This step is needed when OCTEON Linux Kernel is cleaned.
Linux engine build can be cleaned by the following steps.

#  cd /usr/local/Cavium_Networks/OCTEON-SDK/applications/linux_engine

#  make OCTEON_TARGET=linux_64 clean     if ABI N64 is selected
                or
   make OCTEON_TARGET=linux_n32 clean    if ABI N32 is selected
  



09. Testing Engine Using OpenSSL 
=================================
# cd /usr/lib64/engines      if ABI N64 is selected
      or
# cd /usr/lib32/engines      if ABI N32 is selected

a) Running the openssl s_server in echo mode.

    # openssl s_server -engine octeon -cert <CertificateFile> -key <KeyFile>
      This sample application uses OCTEON crypto acceleration.

      Note!! The sample server key and server certificate[server.pem]
             can be found from the following location.

                  /usr/lib32/engines
                    or
                  /usr/lib64/engines

b) Run the client from x86 machine which is in the same LAN
   to connect to the s_server running on OCTEON.

    $ openssl s_client -connect <IP>:<port> 
    where 
           <IP> is IP address of OCTEON used in step 5e.
       <port> is 4433 [default port number on which s_server runs].

c) Type anything on s_client screen, will echo it on the server.
   Type Q to quit on s_client terminal.




10. Testing Engine Using Sample Application
===========================================
A sample application is provided for developers on how to
use engine in their SSL application. Please refer to
"Developers section" for more details.

Testing using this sample application.

        #cd /usr/lib64/engines  if ABI N64 is selected
                or
        #cd /usr/lib32/engines  if ABI N32 is selected

a) Run the sslserver on OCTEON Linux as follows
        # sslserver


b) Run the client from x86 machine which is in the same LAN.

   i) Connecting through Browser

        Type the following in the address bar of your browser.
        
        https://<IP>:<port>
           where 
             <IP> is IP address of OCTEON used in step 5e.
             <port> is 5000 [default port number on which sslserver runs].

        This displays a default page.

   ii) Connecting through openssl client
         $ openssl s_client -connect <IP>:<port>
            where 
            <IP> is IP address of OCTEON used in step 5g.
            <port> is 5000 [default port number on which sslserver runs].




11. Supported Features
======================
   a. Symmetric Algorithms
         3DES
         AES 128
         AES 256
         DES (64 bit)
   b. Digests
         MD5
         SHA1
   c. Key Exchange
         RSA
         DH
         DSA



12 Additional Documentation
===========================
    The additional documentation of OCTEON OpenSSL Engine can be found from
    the following location.

    /usr/local/Cavium_Networks/OCTEON-SDK/applications/linux_engine/docs/index.html




13. Change History
==================
Release 2.0
      - Ported to SDK 2.0.0
      - Removed CRYPTO-CORE dependency
          
Release 0.5
      - Ported to SDK 1.7.0
      - Minor cosmetic changes done in the code.

Release 0.4
      - Added user space modexp support for parts with no issues.
      - MD5/SHA1 bug fixes.
      - Ported to SDK 1.6.0 

Release 0.3
      - Ported to SDK 1.5.0 (build 187)

Release 0.2
      - Installation directory changed.
      - Destroy engine interface implemented.

Release 0.1     
      - Initial Pre-Release


<EOF>
