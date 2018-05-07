/* gpgme.js - Javascript integration for gpgme
 * Copyright (C) 2018 Bundesamt für Sicherheit in der Informationstechnik
 *
 * This file is part of GPGME.
 *
 * GPGME is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * GPGME is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: LGPL-2.1+
 */

var inputvalues = {
    encrypt: {
        good:{
            data : 'Hello World.',
            fingerprint : 'CDC3A2B2860625CCBFC5A5A9FC6D1B604967FC40'
        },
        bad: {
            fingerprint: 'CDC3A2B2860625CCBFC5AAAAAC6D1B604967FC4A'
        }
    },
    init: {
        invalid_startups: [{all_passwords: true}, 'openpgpmode', {api_style:"frankenstein"}]
    }

};

function bigString(megabytes){
    let maxlength = 1024 * 1024 * megabytes;
    let uint = new Uint8Array(maxlength);
    for (let i= 0; i < maxlength; i++){
        uint[i] = Math.random() * Math.floor(256);
    }
    return new TextDecoder('utf-8').decode(uint);
}