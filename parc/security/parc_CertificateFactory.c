/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
//  parc_CertificateFactory.c
//  PARC Library
//
//  Created by Wood, Christopher <Christopher.Wood@parc.com> on 9/2/15.
//  Copyright (c) 2015 Computer Science Laboratory. All rights reserved.
//

#include <config.h>

#include <parc/security/parc_CertificateFactory.h>
#include <parc/security/parc_X509Certificate.h>

#include <parc/algol/parc_Object.h>

struct parc_certificate_factory {
    PARCCertificateType type;
    PARCContainerEncoding encoding;
};

parcObject_ExtendPARCObject(PARCCertificateFactory, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

parcObject_ImplementAcquire(parcCertificateFactory, PARCCertificateFactory);
parcObject_ImplementRelease(parcCertificateFactory, PARCCertificateFactory);

PARCCertificateFactory *
parcCertificateFactory_Create(PARCCertificateType type, PARCContainerEncoding encoding)
{
    PARCCertificateFactory *factory = parcObject_CreateInstance(PARCCertificateFactory);
    factory->type = type;
    factory->encoding = encoding;
    return factory;
}

PARCCertificate *
parcCertificateFactory_CreateCertificateFromFile(PARCCertificateFactory *factory, char *filename, char *password __attribute__((unused)))
{
    if (factory->type == PARCCertificateType_X509 && factory->encoding == PARCContainerEncoding_PEM) {
        PARCX509Certificate *certificate = parcX509Certificate_CreateFromPEMFile(filename);
        PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
        return wrapper;
    }

    // Unsupported configuration
    return NULL;
}

PARCCertificate *
parcCertificateFactory_CreateCertificateFromBuffer(PARCCertificateFactory *factory, PARCBuffer *buffer)
{
    if (factory->type == PARCCertificateType_X509 && factory->encoding == PARCContainerEncoding_DER) {
        PARCX509Certificate *certificate = parcX509Certificate_CreateFromDERBuffer(buffer);

        // This may fail.
        if (certificate == NULL) {
            return NULL;
        }

        PARCCertificate *wrapper = parcCertificate_CreateFromInstance(PARCX509CertificateInterface, certificate);
        return wrapper;
    }

    // Unsupported configuration
    return NULL;
}
