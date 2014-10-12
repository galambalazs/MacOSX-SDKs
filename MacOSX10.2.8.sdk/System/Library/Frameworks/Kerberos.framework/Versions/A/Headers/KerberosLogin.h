/*
 * KerberosLogin.h
 *
 * $Header: /cvs/kfm/KerberosFramework/KerberosLogin/Headers/Kerberos/KerberosLogin.h,v 1.79 2002/06/10 14:14:21 lxs Exp $
 *
 */
 
/* 
 * This file contains part of the login library API. See
 * <http://web.mit.edu/macdev/mit/lib/Login/doc/API.html>
 * for API documentation
 */

#ifndef __KERBEROSLOGIN__
#define __KERBEROSLOGIN__

#if defined(macintosh) || (defined(__MACH__) && defined(__APPLE__))
	#include <TargetConditionals.h>
    #if TARGET_RT_MAC_CFM
        #error "Use KfM 4.0 SDK headers for CFM compilation."
    #endif
#endif
#include <sys/types.h>
#include <Kerberos/krb5.h>
#include <Kerberos/krb.h>

#ifdef __cplusplus
extern "C" {
#endif

#if TARGET_OS_MAC
    #if defined(__MWERKS__)
        #pragma import on
        #pragma enumsalwaysint on
    #endif
    #pragma options align=mac68k
#endif

/* 
 * Kerberos version constants
 */
enum KLEKerberosVersion {
	kerberosVersion_Any		= 0,
	kerberosVersion_V4		= 1,
	kerberosVersion_V5		= 2,
	kerberosVersion_All		= 0xFFFFFFFF
};

/* 
 * Dialog identifier constants 
 */
enum KLEDialogIdentifiers {
	loginLibrary_LoginDialog,
	loginLibrary_OptionsDialog,
	loginLibrary_ChangePasswordDialog,
	loginLibrary_ProgressDialog,
	loginLibrary_PrompterDialog
};

/* 
 * Login dialog items 
 */
enum KLELoginDialogItems {
	loginDialog_Username,
	loginDialog_Password,
	loginDialog_Realm,
	loginDialog_TicketLifetime,
	loginDialog_ForwardableTicket
};

/* 
 * Password dialog items 
 */
enum KLEChangePasswordDialogItems {
	changePasswordDialog_OldPassword,
	changePasswordDialog_NewPassword,
	changePasswordDialog_VerifyPassword
};

/* 
 * Option identifier constants 
 */
enum KLEDefaultLoginOptions {
	/* Dialog state options */
	loginOption_LoginName                     = 'name',
	loginOption_LoginInstance                 = 'inst',
	loginOption_AdvancedLoginMode             = 'adv ',
	loginOption_ShowTicketLifetime            = 'life',
	loginOption_ShowForwardableTicket         = 'forw',
	loginOption_ShowProxiableTicket       	  = 'prox',
	
	/* Initial values and ranges */
	loginOption_RememberPrincipal             = 'prin',
	loginOption_RememberExtras                = 'extr',
	
	loginOption_MinimalTicketLifetime         = '-lif',
	loginOption_MaximalTicketLifetime         = '+lif',
	loginOption_DefaultTicketLifetime         = '0lif',
	loginOption_LongTicketLifetimeDisplay     = 'hms ',
	
	loginOption_DefaultForwardableTicket      = '0fwd',
	loginOption_DefaultProxiableTicket		  = '0prx'
};

/* 
 * Login mode identifier constants (for loginOption_AdvancedLoginMode) 
 */
enum KLELoginMode {
	loginMode_Basic							 = 1,
	loginMode_Advanced						 = 2
};

/* 
 * Realm list constants 
 */
enum KLERealmListIndexes {
	realmList_Start		= 0,
	realmList_End		= 0xFFFF
};

#define klFirstError	19276
#define klLastError		19876

/* 
 * Error codes 
 */
enum KLEStatus {
	klNoErr										=	0,

	/* parameter errors */
	klParameterErr								=	19276,
	klBadPrincipalErr,
	klBadPasswordErr,
	klBadLoginOptionsErr,
	klInvalidVersionErr,
	klCapsLockErr,
    klBadV5ContextErr,
	
    /* Get/SetKerberosOption errors */
	klBufferTooSmallErr							=	19376,
	klBufferTooLargeErr,
	klInvalidOptionErr,
	klBadOptionValueErr,

	/* Runtime Login errors */
	klUserCanceledErr							=	19476,
	klMemFullErr,
	klPreferencesReadErr,
	klPreferencesWriteErr,
	klV5InitializationFailedErr,
	klPrincipalDoesNotExistErr,
	klSystemDefaultDoesNotExistErr,
	klCredentialsExpiredErr,
	klNoRealmsErr,
	klRealmDoesNotExistErr,
	klNoCredentialsErr,
	klCredentialsBadAddressErr,
	klCacheDoesNotExistErr,
    klNoHostnameErr,
    klCredentialsNeedValidationErr,
    
	/* Password changing errors */
	klPasswordMismatchErr						=	19576,
	klInsecurePasswordErr,
	klPasswordChangeFailedErr,
	
	/* Dialog errors */
	klDialogDoesNotExistErr						=	19676,
	klDialogAlreadyExistsErr,
	klNotInForegroundErr,
	klNoAppearanceErr,
	klFatalDialogErr,
	klCarbonUnavailableErr,
        
	/* Login IPC errors */
	klCantContactServerErr						=	19776,
    klCantDisplayUIErr,
    klServerInsecureErr

};

/*
 * Types
 */
 
typedef	int32_t		KLStatus;				/* one of KLEStatus */
typedef	u_int32_t	KLKerberosVersion;		/* one of KLEKerberosVersion */
typedef	u_int32_t	KLDefaultLoginOption;	/* one of KLEDefaultLoginOptions */
typedef	u_int32_t	KLLoginMode;			/* one of KLELoginMode */
typedef	u_int32_t	KLDialogIdentifier;		/* one of KLEDialogIdentifiers */
typedef	u_int32_t	KLIndex;				/* index (used for the realm list) */
typedef	u_int32_t	KLLifetime;				/* Lifetime in seconds */
typedef	u_int32_t	KLTime;					/* Unix time (seconds since 1/1/1970 00:00:00 GMT) */
typedef	u_int32_t	KLSize;					/* size of a buffer (KLG/SetDefaultLoginOptions) or realm list (CountKerberosRealms)	*/
typedef	u_int32_t	KLRefCon;				/* application ref con */
typedef int8_t		KLBoolean;				/* true or false! (unsigned 8 byte value to match Boolean) */
typedef	int16_t		KLSInt16;				/* used for Darwin-compat for KLApplicationOptions */

/* Callback API for Kerberos Login Mach IPC idle filter */
typedef void (*KLIdleCallback) (KLRefCon appData);
#define CallKLIdleCallback(userRoutine, appData) ((userRoutine) (appData))

/* Application options */
typedef struct {
	void *				deprecatedEventFilter;
	KLRefCon 			deprecatedEventFilterAppData;
	KLSInt16 			deprecatedRealmsPopupMenuID;
	KLSInt16 			deprecatedLoginModeMenuID;
} KLApplicationOptions;

/* Principal information */
struct OpaqueKLPrincipal;
typedef struct OpaqueKLPrincipal 	* KLPrincipal;

/* Login Options */
struct OpaqueKLLoginOptions;
typedef struct OpaqueKLLoginOptions	* KLLoginOptions;


/*
 *
 * Functions
 *
 */
 
/* Deprecated functions -- provided for compatibility with KfM 4.0 */
#ifdef KERBEROSLOGIN_DEPRECATED

KLStatus KLAcquireTickets (				
		KLPrincipal		  inPrincipal, 
		KLPrincipal		 *outPrincipal, 
		char 			**outCredCacheName);

KLStatus KLAcquireNewTickets (
		KLPrincipal 	  inPrincipal, 
		KLPrincipal 	 *outPrincipal, 
		char 		**outCredCacheName);
        
KLStatus KLAcquireTicketsWithPassword (
		KLPrincipal		  inPrincipal,
		KLLoginOptions	          inLoginOptions,
		const char		 *inPassword,
		char 			**outCredCacheName);

KLStatus KLAcquireNewTicketsWithPassword (
		KLPrincipal		  inPrincipal,
		KLLoginOptions	          inLoginOptions,
		const char		 *inPassword,
		char 			**outCredCacheName);

#endif

/* Kerberos Login high-level API */
KLStatus KLAcquireInitialTickets (				
		KLPrincipal		  inPrincipal, 
		KLLoginOptions	  inLoginOptions,
		KLPrincipal		 *outPrincipal, 
		char 			**outCredCacheName);

KLStatus KLAcquireNewInitialTickets (
		KLPrincipal 	  inPrincipal, 
		KLLoginOptions	  inLoginOptions,
		KLPrincipal 	 *outPrincipal, 
		char 			**outCredCacheName);

KLStatus KLDestroyTickets (KLPrincipal inPrincipal);

KLStatus KLChangePassword (KLPrincipal inPrincipal);


/* Kerberos Login dialog low level functions */

KLStatus KLAcquireInitialTicketsWithPassword (
		KLPrincipal		  inPrincipal,
		KLLoginOptions	  inLoginOptions,
		const char		 *inPassword,
		char 			**outCredCacheName);

KLStatus KLAcquireNewInitialTicketsWithPassword (
		KLPrincipal		  inPrincipal,
		KLLoginOptions	  inLoginOptions,
		const char		 *inPassword,
		char 			**outCredCacheName);

KLStatus KLAcquireNewInitialTicketCredentialsWithPassword (
		KLPrincipal         inPrincipal,
		KLLoginOptions      inLoginOptions,
		const char         *inPassword,
		krb5_context        inV5Context,
		KLBoolean          *outGotV4Credentials,
		KLBoolean          *outGotV5Credentials,
		CREDENTIALS        *outV4Credentials,
		krb5_creds         *outV5Credentials);

KLStatus KLVerifyInitialTickets (
        KLPrincipal 	    inPrincipal,
        KLBoolean           inFailIfNoHostKey,
        char 	          **outCredCacheName);
        
KLStatus KLVerifyInitialTicketCredentials (
        CREDENTIALS        *inV4Credentials,
        krb5_creds 	       *inV5Credentials,
        KLBoolean           inFailIfNoHostKey);

KLStatus KLAcquireNewInitialTicketsWithKeytab (
        KLPrincipal			inPrincipal,
        KLLoginOptions		inLoginOptions,
        const char 		   *inKeytabName,
        char			  **outCredCacheName);
        
KLStatus KLRenewInitialTickets (
        KLPrincipal 	  inPrincipal,
		KLLoginOptions	  inLoginOptions,
		KLPrincipal		 *outPrincipal, 
		char 			**outCredCacheName);

KLStatus KLValidateInitialTickets (
        KLPrincipal 	  inPrincipal,
		KLLoginOptions	  inLoginOptions,
		char 			**outCredCacheName);
        
KLStatus KLLastChangedTime (KLTime	*outLastChangedTime);

KLStatus KLCacheHasValidTickets (
		KLPrincipal		 	  inPrincipal,
		KLKerberosVersion	  inKerberosVersion,
		KLBoolean 			 *outFoundValidTickets,
		KLPrincipal			 *outPrincipal,
		char				**outCredCacheName);

KLStatus KLTicketStartTime (
		KLPrincipal					 inPrincipal,
		KLKerberosVersion			 inKerberosVersion,
		KLTime						*outStartTime);

KLStatus KLTicketExpirationTime (
		KLPrincipal			 inPrincipal,
		KLKerberosVersion	 inKerberosVersion,
		KLTime				*outExpirationTime);
     
KLStatus KLSetSystemDefaultCache (KLPrincipal inPrincipal);

KLStatus KLHandleError (
		KLStatus				inError,
		KLDialogIdentifier		inDialogIdentifier,
		KLBoolean					inShowAlert);

KLStatus KLGetErrorString (
		KLStatus		  inError,
		char			**outErrorString);

KLStatus KLCancelAllDialogs (void);

/* Kerberos change password dialog low level functions */

KLStatus KLChangePasswordWithPasswords (
		KLPrincipal		  inPrincipal,
		const char		 *inOldPassword,
		const char		 *inNewPassword,
		KLBoolean 		 *outRejected,
		char 			**outRejectionError,
		char 			**outRejectionDescription);

/* Application Configuration functions */
KLStatus KLSetIdleCallback (const KLIdleCallback inCallback, const KLRefCon inRefCon);
KLStatus KLGetIdleCallback (KLIdleCallback* inCallback, KLRefCon* inRefCon);

/* These functions are deprecated on Mac OS X */
KLStatus KLSetApplicationOptions (const KLApplicationOptions *inAppOptions);
KLStatus KLGetApplicationOptions (KLApplicationOptions *outAppOptions);

/* Library configuration functions */
KLStatus KLGetDefaultLoginOption (     
		const KLDefaultLoginOption	 inOption,
		void						*ioBuffer,
		KLSize						*ioBufferSize);

KLStatus KLSetDefaultLoginOption (
		const KLDefaultLoginOption	 inOption,
		const void					*inBuffer,
		const KLSize				 inBufferSize);

/* Realm configuration functions */

KLStatus KLFindKerberosRealmByName (
		const char		*inRealmName,
		KLIndex			*outIndex);

KLStatus KLGetKerberosRealm (
		KLIndex			  inIndex,
		char			**outRealmName);

KLStatus KLSetKerberosRealm (
		KLIndex			 inIndex,
		const char		*inRealmName);

KLStatus KLRemoveKerberosRealm (KLIndex inIndex);

KLStatus KLInsertKerberosRealm (
		KLIndex			 inInsertBeforeIndex,
		const char		*inRealmName);
		
KLStatus KLRemoveAllKerberosRealms (void);
		
KLSize KLCountKerberosRealms (void);
        
KLStatus KLGetKerberosDefaultRealm(KLIndex *outIndex);
     
KLStatus KLGetKerberosDefaultRealmByName (char **outRealmName);
        
KLStatus KLSetKerberosDefaultRealm (KLIndex inIndex);
        
KLStatus KLSetKerberosDefaultRealmByName (const char *inRealm);

/* KLPrincipal functions */

KLStatus KLCreatePrincipalFromTriplet(
		const char		*inName,
		const char		*inInstance,
		const char		*inRealm,
		KLPrincipal 	*outPrincipal);

KLStatus KLCreatePrincipalFromString(
		const char				*inFullPrincipal,
		KLKerberosVersion		 inKerberosVersion,
		KLPrincipal				*outPrincipal);
    
KLStatus KLGetTripletFromPrincipal(
		KLPrincipal		  inPrincipal,
		char			**outName,
		char			**outInstance,
		char			**outRealm);

KLStatus KLGetStringFromPrincipal(
		KLPrincipal			  inPrincipal,
		KLKerberosVersion	  inKerberosVersion,
		char				**outFullPrincipal);

KLStatus KLGetDisplayStringFromPrincipal(
		KLPrincipal			  inPrincipal,
		KLKerberosVersion	  inKerberosVersion,
		char				**outFullPrincipal);

KLStatus KLComparePrincipal(
		KLPrincipal		 inFirstPrincipal,
		KLPrincipal		 inSecondPrincipal,
		KLBoolean		*outAreEquivalent);

KLStatus KLDisposePrincipal(KLPrincipal inPrincipal);

/* KLLoginOptions functions */

KLStatus KLCreateLoginOptions (KLLoginOptions 	*outOptions);

KLStatus KLLoginOptionsSetTicketLifetime (
		KLLoginOptions	ioOptions,
		KLLifetime		inTicketLifetime);

KLStatus KLLoginOptionsSetForwardable (
		KLLoginOptions	ioOptions,
		KLBoolean		inForwardable);
		
KLStatus KLLoginOptionsSetProxiable (
		KLLoginOptions	ioOptions,
		KLBoolean		inProxiable);
		
KLStatus KLLoginOptionsSetRenewableLifetime (
		KLLoginOptions	ioOptions,
		KLLifetime		inRenewableLifetime);

KLStatus KLLoginOptionsSetAddressless (
		KLLoginOptions	ioOptions,
        KLBoolean 		inAddressless);
        
KLStatus KLLoginOptionsSetTicketStartTime (
		KLLoginOptions	ioOptions,
		KLTime			inStartTime);

KLStatus KLLoginOptionsSetServiceName (
		KLLoginOptions	 ioOptions,
		const char		*inServiceName);
        
KLStatus KLDisposeLoginOptions(KLLoginOptions ioOptions);


/* Misc function */

KLStatus KLDisposeString(char *inStringToDispose);

#if TARGET_OS_MAC
    #if defined(__MWERKS__)
        #pragma enumsalwaysint reset
        #pragma import reset
    #endif
	#pragma options align=reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __KERBEROSLOGIN__ */
