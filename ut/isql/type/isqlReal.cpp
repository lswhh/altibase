/** 
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
/***********************************************************************
 * $Id$
 **********************************************************************/

#include <utISPApi.h>
#include <isqlTypes.h>

isqlReal::isqlReal()
{
    mCType = SQL_C_FLOAT;
}

IDE_RC isqlReal::initBuffer()
{
    mValue = (SChar *) idlOS::malloc(ID_SIZEOF(SFloat));
    IDE_TEST(mValue == NULL);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void isqlReal::initDisplaySize()
{
    mDisplaySize = REAL_DISPLAY_SIZE;
}

void isqlReal::Reformat()
{
    SChar  sTmp[16];
    SFloat sFloat;
#ifdef VC_WIN32
    SChar *sTarget;
    SInt   sDest;
#endif

    if ( mInd == SQL_NULL_DATA )
    {
        mBuf[0] = '\0';
    }
    else
    {
        sFloat = *(SFloat *)mValue;

        // fix PR-12295
        // 0에 가까운 작은 값은 0으로 출력함.
        if( ( sFloat < 1E-7 ) &&
            ( sFloat > -1E-7 ) )
        {
            idlOS::sprintf(sTmp, "0");
        }
        else
        {
            idlOS::sprintf(sTmp, "%"ID_FLOAT_G_FMT"", sFloat);
#ifdef VC_WIN32 // ex> UNIX:WIN32 = 3.06110e+04:3.06110e+004
            sTarget = strstr(sTmp, "+0");
            if ( sTarget == NULL )
            {
                if ( strstr(sTmp, "-0.") != NULL )
                {
                    sTarget = NULL;
                }
                else
                {
                    sTarget = strstr(sTmp, "-0");
                }
            }
            if ( sTarget != NULL )
            {
                sDest = (int)(sTarget - sTmp);
                idlOS::memmove(sTmp+sDest+1,
                               sTmp+sDest+2,
                               idlOS::strlen(sTmp+sDest+2)+1);
            }
#endif
        }
        idlOS::sprintf(mBuf, "%s", sTmp);
    }
    mCurr = mBuf;
    mCurrLen = idlOS::strlen(mCurr);
}
