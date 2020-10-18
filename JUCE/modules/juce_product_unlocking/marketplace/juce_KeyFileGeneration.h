/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   By using JUCE, you agree to the terms of both the JUCE 5 End-User License
   Agreement and JUCE 5 Privacy Policy (both updated and effective as of the
   27th April 2017).

   End User License Agreement: www.juce.com/juce-5-licence
   Privacy Policy: www.juce.com/juce-5-privacy-policy

   Or: You may also use this code under the terms of the GPL v3 (see
   www.gnu.org/licenses).

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

/**
    Contains static utilities for generating key-files that can be unlocked by
    the OnlineUnlockStatus class.

    @tags{ProductUnlocking}
*/
class JUCE_API  KeyGeneration
{
public:
    /**
        Generates the content of a key-file which can be sent to a user's machine to
        unlock a product.

        The returned value is a block of text containing an RSA-encoded block, followed
        by some human-readable details. If you pass this block of text to
        OnlineUnlockStatus::applyKeyFile(), it will decrypt it, and if the
        key matches and the machine numbers match, it will unlock that machine.

        Typically the way you'd use this on a server would be to build a small executable
        that simply calls this method and prints the result, so that the webserver can
        use this as a reply to the product's auto-registration mechanism. The
        keyGenerationAppMain() function is an example of how to build such a function.

        @see OnlineUnlockStatus
    */
    static String JUCE_CALLTYPE generateKeyFile (const String& appName,
                                                 const String& userEmail,
                                                 const String& userName,
                                                 const String& machineNumbers,
                                                 const RSAKey& privateKey);

    /** Similar to the above key file generation method but with an expiry time.
        You must supply a Time after which this key file should no longer be considered as active.

        N.B. when an app is unlocked with an expiring key file, OnlineUnlockStatus::isUnlocked will
        still return false. You must then check OnlineUnlockStatus::getExpiryTime to see if this
        expiring key file is still in date and act accordingly.

        @see OnlineUnlockStatus
    */
    static String JUCE_CALLTYPE generateExpiringKeyFile (const String& appName,
                                                         const String& userEmail,
                                                         const String& userName,
                                                         const String& machineNumbers,
                                                         const Time expiryTime,
                                                         const RSAKey& privateKey);

    //==============================================================================
    /** This is a simple implementation of a key-generator that you could easily wrap in
        a command-line main() function for use on your server.

        So for example you might use this in a command line app called "unlocker" and
        then call it like this:

        unlocker MyGreatApp Joe_Bloggs joebloggs@foobar.com 1234abcd,95432ff 22d9aec92d986dd1,923ad49e9e7ff294c
    */
    static inline int keyGenerationAppMain (int argc, char* argv[])
    {
        StringArray args;
        for (int i = 1; i < argc; ++i)
            args.add (argv[i]);

        if (args.size() != 5)
        {
            std::cout << "Requires 5 arguments: app-name user-email username machine-numbers private-key" << std::endl
                      << "  app-name:         name of the product being unlocked" << std::endl
                      << "  user-email:       user's email address" << std::endl
                      << "  username:         name of the user. Careful not to allow any spaces!" << std::endl
                      << "  machine-numbers:  a comma- or semicolon-separated list of all machine ID strings this user can run this product on (no whitespace between items!)" << std::endl
                      << "  private-key:      the RSA private key corresponding to the public key you've used in the app" << std::endl
                      << std::endl;

            return 1;
        }

        if (! args[4].containsChar (','))
        {
            std::cout << "Not a valid RSA key!" << std::endl;
            return 1;
        }

        std::cout << generateKeyFile (args[0], args[1], args[2], args[3], RSAKey (args[4])) << std::endl;
        return 0;
    }
};

} // namespace juce
