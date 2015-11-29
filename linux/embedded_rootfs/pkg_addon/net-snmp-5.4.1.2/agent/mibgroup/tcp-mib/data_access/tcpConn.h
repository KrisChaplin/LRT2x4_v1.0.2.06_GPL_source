/*
 * tcpConn data access header
 *
 * $Id: tcpConn.h 1786 2008-11-10 13:36:41Z richie $
 */
/**---------------------------------------------------------------------*/
/*
 * configure required files
 *
 * Notes:
 *
 * 1) prefer functionality over platform, where possible. If a method
 *    is available for multiple platforms, test that first. That way
 *    when a new platform is ported, it won't need a new test here.
 *
 * 2) don't do detail requirements here. If, for example,
 *    HPUX11 had different reuirements than other HPUX, that should
 *    be handled in the *_hpux.h header file.
 */
config_require(tcp-mib/data_access/tcpConn_common)
#if defined( linux )
config_require(tcp-mib/data_access/tcpConn_linux)
#elif defined( solaris2 )
config_require(tcp-mib/data_access/tcpConn_solaris2)
#else
#   define NETSNMP_TCPCONN_COMMON_ONLY
#endif

