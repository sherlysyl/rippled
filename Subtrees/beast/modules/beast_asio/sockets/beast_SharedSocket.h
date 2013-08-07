//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef BEAST_SHAREDSOCKET_H_INCLUDED
#define BEAST_SHAREDSOCKET_H_INCLUDED

/** A Socket interface with reference counting.

    You can keep a pointer to the base class so that you don't have
    to see the template or underlying object implementation.

    @see SharedSocketTYpe, SharedObjectPtr
*/
/** @{ */
class SharedSocket
    : public SharedObject
    , public virtual Socket
{
public:
    /** Store your SharedSocket in one of these! */
    typedef SharedObjectPtr <SharedSocket> Ptr;
};

//------------------------------------------------------------------------------

/** A RAII container for wrapping an object as a Socket.

    To use this, construct the class with an instance of your object
    created with operator new. The constructor will take ownership,
    and delete it when the last reference is removed. For example:

    @code

    boost::asio::io_service ios;
    boost::asio::ssl:context ctx;

    SharedSocket::Ptr mySocket (
        new (boost::asio::ssl::stream (ios, ctx)));

    mySocket->handshake ();

    @endcode

    @see SharedSocket
*/
template <class Object>
class SharedSocketType
    : public SharedSocket
    , public SocketWrapper <Object>
{
public:
    /** Create the shared socket.
        This takes posession of the object, which will be deleted
        when the last reference goes away.
    */
    SharedSocketType (Object* object)
        : SocketWrapper <Object> (object)
        , m_object (object)
    {
    }

private:
    ScopedPointer <Object> m_object;
};
/** @} */

#endif