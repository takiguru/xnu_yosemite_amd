/*
 * Copyright (c) 1998-2012 Apple Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */
/*
 * Copyright (c) 1998 Apple Inc.  All rights reserved. 
 *
 * HISTORY
 *
 */


#ifndef _IOKIT_IOCATALOGUE_H
#define _IOKIT_IOCATALOGUE_H

#include <libkern/c++/OSObject.h>
#include <libkern/c++/OSCollectionIterator.h>
#include <libkern/c++/OSArray.h>
#include <libkern/c++/OSDictionary.h>
#include <IOKit/IOLocks.h>
#include <sys/cdefs.h>

#include <IOKit/IOKitServer.h>

class IOService;

/*!
    @class IOCatalogue
    @abstract In-kernel database for IOKit driver personalities.
    @discussion The IOCatalogue is a database which contains all IOKit driver personalities.  IOService uses this resource when matching devices to their associated drivers.
*/
class IOCatalogue : public OSObject
{
    OSDeclareDefaultStructors(IOCatalogue)
    
private:
    IORWLock *               lock;
    SInt32                   generation;
    OSDictionary           * personalities;
    OSArray * arrayForPersonality(OSDictionary * dict);
    void addPersonality(OSDictionary * dict);

public:
    /*!
        @function initialize
        @abstract Creates and initializes the database object and poputates it with in-kernel driver personalities.
    */
    static void initialize( void );
    
    /*!
        @function init
        @abstract Initializes the database object.
        @param initArray  The initial array of driver personalities to populate the database.
    */
    bool init( OSArray * initArray );
    
    /*!
        @function free
        @abstract Cleans up the database and deallocates memory allocated at initialization.  This is never called in normal operation of the system.
    */
    void free( void );
    
    /*!
        @function findDrivers
        @abstract This is the primary entry point for IOService.
        @param service
        @param generationCount  Returns a reference to the generation count of the database. The generation count increases only when personalities are added to the database *and* IOService matching has been initiated.
        @result Returns an ordered set of driver personalities ranked on probe-scores.  The ordered set must be released by the receiver.
    */
    OSOrderedSet * findDrivers( IOService * service, SInt32 * generationCount );
    
    /*!
        @function findDrivers
        @abstract A more general purpose interface which allows one to retreive driver personalities based the intersection of the 'matching' dictionary and the personality's own property list.
        @param matching  A dictionary containing only keys and values which are to be used for matching. For example, a matching dictionary containing 'IOProviderClass'='IOPCIDevice' will return all personalities with an IOProviderClass key and a value of IOPCIDevice.
        @param generationCount  Returns a reference to the current generation of the database. The generation count increases only when personalities are added to the database *and* IOService matching has been initiated.
        @result Returns an ordered set of driver personalities ranked on probe-scores. The ordered set must be released by the receiver.
    */
    OSOrderedSet * findDrivers( OSDictionary * matching, SInt32 * generationCount );
    
    /*!
        @function addDrivers
        @abstract Adds an array of driver personalities to the database.
        @param array Array of driver personalities to be added to the database.
        @param doNubMatchng Start matching process after personalities have been added.
        @result Returns true if driver personality was added to the database successfully. Failure is due to a memory allocation failure.
    */
    bool addDrivers( OSArray * array, bool doNubMatching = true );
    
    /*!
        @function removeDrivers
        @abstract Remove driver personalities from the database based on matching information provided.
        @param matching  A dictionary whose keys and values are used for matching personalities in the database.  For example, a matching dictionary containing a 'IOProviderClass' key with the value 'IOPCIDevice' will remove all personalities which have the key 'IOProviderClass' equal to 'IOPCIDevice'.
        @param doNubMatchng Start matching process after personalities have been removed.  Matching criteria is based on IOProviderClass of those personalities which were removed.  This is to allow drivers which haven't been matched to match against NUB's which were blocked by the previous personalities. 
        @result Returns true if personality was removed successfully. Failure is due to a memory allocation failure.
    */
    bool removeDrivers( OSDictionary * matching, bool doNubMatching = true );
    
    /*!
        @function getGenerationCount
        @abstract Get the current generation count of the database.
    */
    SInt32 getGenerationCount( void ) const;

    /*!
        @function isModuleLoaded
        @abstract Reports if a kernel module has been loaded.
        @param moduleName  Name of the module.
        @result Returns true if the associated kernel module has been loaded into the kernel.
    */
    bool isModuleLoaded( OSString * moduleName ) const;

    /*!
        @function isModuleLoaded
        @abstract Reports if a kernel module has been loaded.
        @param moduleName  Name of the module.
        @result Returns true if the associated kernel module has been loaded into the kernel.
    */
    bool isModuleLoaded( const char * moduleName ) const;
    
    /*!
        @function isModuleLoaded
        @abstract Reports if a kernel module has been loaded for a particular personality.
        @param driver  A driver personality's property list.
        @result Returns true if the associated kernel module has been loaded into the kernel for a particular driver personality on which it depends.
    */
    bool isModuleLoaded( OSDictionary * driver ) const;
    
    /*!
        @function moduleHasLoaded
        @abstract Callback function called after a IOKit dependent kernel module is loaded.
        @param name  Name of the kernel module.
    */
    void moduleHasLoaded( OSString * name );
    
    /*!
        @function moduleHasLoaded
        @abstract Callback function called after a IOKit dependent kernel module is loaded.
        @param name  Name of the kernel module.
    */
    void moduleHasLoaded( const char * name );

    /*!
        @function terminateDrivers
        @abstract Terminates all instances of a driver which match the contents of the matching dictionary. Does not unload module.
        @param matching  A dictionary whose keys and values are used for matching personalities in the database.  For example, a matching dictionary containing a 'IOProviderClass' key with the value 'IOPCIDevice' will cause termination for all instances whose personalities have the key 'IOProviderClass' equal to 'IOPCIDevice'.
     */
    IOReturn terminateDrivers( OSDictionary * matching );

    /*!
        @function terminateDriversForModule
        @abstract Terminates all instances of a driver which depends on a particular module and unloads the module.
        @param moduleName Name of the module which is used to determine which driver instances to terminate and unload.
        @param unload Flag to cause the actual unloading of the module.
     */
    IOReturn terminateDriversForModule( OSString * moduleName, bool unload = true);

    /*!
        @function terminateDriversForModule
        @abstract Terminates all instances of a driver which depends on a particular module and unloads the module.
        @param moduleName Name of the module which is used to determine which driver instances to terminate and unload.
        @param unload Flag to cause the actual unloading of the module.
     */
    IOReturn terminateDriversForModule( const char * moduleName, bool unload = true);

    /*!
        @function startMatching
        @abstract Starts an IOService matching thread where matching keys and values are provided by the matching dictionary.
        @param matching  A dictionary whose keys and values are used for matching personalities in the database.  For example, a matching dictionary containing a 'IOProviderClass' key with the value 'IOPCIDevice' will start matching for all personalities which have the key 'IOProviderClass' equal to 'IOPCIDevice'.
     */
    bool startMatching( OSDictionary * matching );

    /*!
        @function reset
        @abstract Return the Catalogue to its initial state.
        @discussion
        Should only be used by kextd just before it sends all kext personalities down during a rescan.
    */
    void reset(void);

    /*!
        @function resetAndAddDrivers
        @abstract Replace personalities in IOCatalog with those provided.
        @discussion
        Resets the catalogue with a new set of drivers, preserving matching originals to keep wired memory usage down.
    */
    bool resetAndAddDrivers(OSArray * drivers, bool doNubMatching = true);

    /*!
        @function serialize
        @abstract Serializes the catalog for transport to the user.
        @param s The serializer object.
        @result Returns false if unable to serialize database, most likely due to memory shortage.
     */
    virtual bool serialize(OSSerialize * s) const;

    bool serializeData(IOOptionBits kind, OSSerialize * s) const;

/* This stuff is no longer used at all we keep it around for i386
 * binary compatibility only. Symbols are no longer exported.
 */

private:

    /*!
        @function unloadModule
        @abstract Unloads the reqested module if no driver instances are currently depending on it.
        @param moduleName An OSString containing the name of the module to unload.
     */
    IOReturn unloadModule( OSString * moduleName ) const;

    IOReturn _removeDrivers(OSDictionary * matching);
    IOReturn _terminateDrivers(OSDictionary * matching);
};

extern const OSSymbol * gIOClassKey;
extern const OSSymbol * gIOProbeScoreKey;
extern IOCatalogue    * gIOCatalogue;

extern "C" {
    /* kaitek: see ::addDrivers() and StartIOKit() for more information about the built-in kernel
     * kext blacklist. */
    /* AnV - Added configurable blacklist mods */
    typedef struct {
        const char *name;
        uint32_t hits;
    } blacklist_mod_t;
    typedef struct {
        char name[256];
        uint32_t hits;
    } blacklist_confmod_t;
    extern boolean_t blacklistEnabled;
    extern boolean_t confblacklistEnabled;
    extern blacklist_mod_t blacklistMods[];
    extern blacklist_confmod_t confblacklistMods[16];
    extern uint32_t confblacklistCount;
};

#endif /* ! _IOKIT_IOCATALOGUE_H */
