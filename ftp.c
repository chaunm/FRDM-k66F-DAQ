#include "net_config.h"
#include "ftp/ftp_client.h"
#include "ftp.h"

#if (USERDEF_CLIENT_FTP == ENABLED)
error_t ftpClientTest(void);
#endif

#if (USERDEF_CLIENT_FTP == ENABLED)
/**
* @brief FTP client test routine
* @return Error code
**/

error_t ftpClientTest(void)
{
    error_t error;
    size_t length;
    IpAddr ipAddr;
    FtpClientContext ftpContext;
    static char_t buffer[256];
    
    //Debug message
    TRACE_INFO("\r\n\r\nResolving server name...\r\n");
    error = getHostByName(NULL, "192.168.100.25", &ipAddr, 0);
    if(error)
    {
        //Debug message
        TRACE_INFO("Failed to resolve server name!\r\n");
        return error;
    }
    
    //Debug message
    TRACE_INFO("Connecting to FTP server %s\r\n", ipAddrToString(&ipAddr, NULL));
    //Connect to the FTP server
    error = ftpConnect(&ftpContext, NULL, &ipAddr, 21, FTP_NO_SECURITY | FTP_PASSIVE_MODE);
    
    if(error)
    {
        //Debug message
        TRACE_INFO("Failed to connect to FTP server!\r\n");
        //Exit immediately
        return error;
    }
    
    //Debug message
    TRACE_INFO("Successful connection\r\n");
    do
    {
        //Login to the FTP server using the provided username and password
        error = ftpLogin(&ftpContext, "sonlq4", "123", "");
        if(error) break;
        //Open file
        error = ftpOpenFile(&ftpContext, "FRDM-K66F.hex", FTP_FOR_READING | FTP_BINARY_TYPE);
        if(error) break;
        
        while(1)
        {
            //Read data
            error = ftpReadFile(&ftpContext, buffer, sizeof(buffer) - 1, &length, 0);
            //End of file?
            if(error) break;
            
            //terminate the string with a NULL character
            buffer[length] = '\0';
            TRACE_INFO("%s", buffer);
        }
        
        TRACE_INFO("\r\n");
        //Close the file
        error = ftpCloseFile(&ftpContext);
        
        //End of exception handling block
    } while(0);
    
    //Close the connection
    ftpClose(&ftpContext);
    //Debug message
    TRACE_INFO("Connection closed...\r\n");
    
    //Return status code
    return error;
}
#endif