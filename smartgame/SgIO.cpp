//----------------------------------------------------------------------------
/** @file SgIO.cpp
    @see SgIO.h
 */
//----------------------------------------------------------------------------

#include "SgSystem.h"
#include "SgIO.h"

#include <fstream>
#include <iostream>
#include <memory>

using namespace std;

//----------------------------------------------------------------------------

/** Null stream.
    This file stream will never be opened and acts as a null stream
    for SgDebug().
*/
static ofstream s_nullStream;

static auto_ptr<ofstream> s_fileStream;

#if UNIX 
#define textoutputstream cerr
#else
#define textoutputstream cout
#endif

ostream* g_debugStrPtr(&textoutputstream);

std::ostream& SgDebug()
{
    if (! g_debugStrPtr->good())
    {
        // does not just use a direct ASSERT(g_debugStrPtr->good())
        // in order to allow a breakpoint to be set on the line below.
        SG_ASSERT(false);
    }
    return *g_debugStrPtr;
}

//----------------------------------------------------------------------------

void SgDebugToWindow()
{
    g_debugStrPtr = &textoutputstream;
}

void SgDebugToFile(const char* filename)
{
    if (s_fileStream.get() == 0)
        s_fileStream.reset(new ofstream(filename, ios::app));
    g_debugStrPtr = s_fileStream.get();
}

void SgDebugToNull()
{
    g_debugStrPtr = &s_nullStream;
}

ostream* SgSwapDebugStr(ostream* newStr)
{
    ostream* t = g_debugStrPtr;
    g_debugStrPtr = newStr;
    return t;
}

//----------------------------------------------------------------------------

SgDebugToNewFile::SgDebugToNewFile(const char* filename)
    : m_old(SgSwapDebugStr(new ofstream(filename, ios::app)))
{
}

SgDebugToNewFile::SgDebugToNewFile()
    : m_old(NULL)
{
}

void SgDebugToNewFile::SetFile(const char* filename)
{
    m_old = SgSwapDebugStr(new ofstream(filename, ios::app));
}

SgDebugToNewFile::~SgDebugToNewFile()
{
    if (m_old)
    {
        ostream* t = SgSwapDebugStr(m_old);
        delete t;
    }
}

//----------------------------------------------------------------------------

SgDebugToString::SgDebugToString(bool writeToOldDebugStr)
    : m_writeToOldDebugStr(writeToOldDebugStr)
{
    m_old = SgSwapDebugStr(&m_str);
}

SgDebugToString::~SgDebugToString()
{
    if (m_writeToOldDebugStr)
        (*m_old) << GetString();
    SgSwapDebugStr(m_old);
}

//----------------------------------------------------------------------------

