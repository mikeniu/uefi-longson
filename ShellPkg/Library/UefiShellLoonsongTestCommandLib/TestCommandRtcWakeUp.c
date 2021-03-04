/** @file

  Copyright (c) 2018 Loongson Technology Corporation Limited (www.loongson.cn).
  All intellectual property rights(Copyright, Patent and Trademark) reserved.

  Any violations of copyright or other intellectual property rights of the Loongson
  Technology Corporation Limited will be held accountable in accordance with the law,
  if you (or any of your subsidiaries, corporate affiliates or agents) initiate directly
  or indirectly any Intellectual Property Assertion or Intellectual Property Litigation:
  (i) against Loongson Technology Corporation Limited or any of its subsidiaries or corporate affiliates,
  (ii) against any party if such Intellectual Property Assertion or Intellectual Property Litigation arises
  in whole or in part from any software, technology, product or service of Loongson Technology Corporation Limited
  or any of its subsidiaries or corporate affiliates, or (iii) against any party relating to the Software.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION).

 **/
#include "UefiLoongsonTestCommandLib.h"

EFI_STATUS
EFIAPI
CalculateTimeIncrease(
    IN   UINT16    IncreaseYear,
    IN   UINT16    IncreaseMonth,
    IN   UINT16    IncreaseDay,
    IN   UINT16    IncreaseHour,
    IN   UINT16    IncreaseMinute,
    IN   UINT16    IncreaseSecond,
    IN   EFI_TIME  *TimeCurrent,
    OUT  EFI_TIME  *TimeFuture
    );

#define  DELAY_SHUTDOWN   (4)
#define  isLeapYear(Year)    ((0 == Year%4)&&(0 != Year%100)) ||(0 == Year %400)? TRUE :FALSE
#define  IncreaseYearFunc(Years, CurrentP, FutureP)      CalculateTimeIncrease(Years,0,0,0,0,0, CurrentP, FutureP)
#define  IncreaseMonthFunc(Months, CurrentP, FutureP)    CalculateTimeIncrease(0,Months,0,0,0,0, CurrentP, FutureP)
#define  IncreaseDayFunc(Days, CurrentP, FutureP)        CalculateTimeIncrease(0,0,Days,0,0,0, CurrentP, FutureP)
#define  IncreaseHourFunc(Hours, CurrentP, FutureP)      CalculateTimeIncrease(0,0,0,Hours,0,0, CurrentP, FutureP)
#define  IncreaseMinuteFunc(Minutes, CurrentP, FutureP)  CalculateTimeIncrease(0,0,0,0,Minutes,0, CurrentP, FutureP)
#define  IncreaseSecondFunc(Seconds, CurrentP, FutureP)  CalculateTimeIncrease(0,0,0,0,0,Seconds, CurrentP, FutureP)


STATIC CONST SHELL_PARAM_ITEM ParamList[] = {
  {L"-h", TypeValue},
  {L"-m", TypeValue},
  {L"-s", TypeValue},
  {NULL, TypeMax}
};

/**
 Function for Judge how many days there are in a month.

 @param[in] Year    Which year.
 @param[in] Month   Which Month.

 @retval  Zero      Failure.
 @retval  UINT32    Sucessfully.
 **/
UINT32
  EFIAPI
NumberOfDaysForMonth(
    IN UINT16  Year,
    IN UINT16  Month
    )
{
  if(Year <= 0 || Month > 12 || Month < 1){
    return 0;
  }
  switch (Month){
  case 1:case 3:case 5:case 7:case 8:case 10:case 12:
    return 31;
  case 4:case 6:case 9:case 11:
    return 30;
  case 2:
    if(isLeapYear(Year)){
      return 29;
    }else{
      return 28;
    }
  default:
    return 0;
  }
}

/**
  Calculate Time for future.

  @param[in] IncreaseYear     Change Value of Year.
  @param[in] IncreaseMonth    Change Value of Month.
  @param[in] IncreaseDay      Change Value of Day.
  @param[in] IncreaseHour     Change Value of Hour.
  @param[in] IncreaseMinute   Change Value of Minute.
  @param[in] IncreaseSecond   Change Value of Second.
  @param[in] TimeCurrent      Pointer to the Current time struct.
  @param[in] TimeFuture       Pointer to the buffer used for save future time struct.

  @retval EFI_SUCCESS                 The future time be Calculated sucessfully.
  @retval EFI_INVALID_PARAMETER       A parameter was invalid.

 **/
EFI_STATUS
  EFIAPI
CalculateTimeIncrease(
    IN   UINT16    IncreaseYear,
    IN   UINT16    IncreaseMonth,
    IN   UINT16    IncreaseDay,
    IN   UINT16    IncreaseHour,
    IN   UINT16    IncreaseMinute,
    IN   UINT16    IncreaseSecond,
    IN   EFI_TIME  *TimeCurrent,
    OUT  EFI_TIME  *TimeFuture
    )
{
  UINT32 NumberOfDay;
  UINT32 Temp;
  UINT8  Flag;

  Flag = 0;
  if(TimeCurrent == NULL || TimeFuture == NULL){
    return EFI_INVALID_PARAMETER;
  }

  NumberOfDay = NumberOfDaysForMonth(TimeCurrent->Year, TimeCurrent->Month);
  if(!NumberOfDay){
    return EFI_INVALID_PARAMETER;
  }

  // Second
  if(IncreaseSecond){
    Temp = TimeCurrent->Second + IncreaseSecond;
    TimeCurrent->Second = Temp%60;
    Flag = Temp/60;
  }

  // Minute
  if(IncreaseMinute || Flag){
    Temp = TimeCurrent->Minute + IncreaseMinute  + Flag;
    TimeCurrent->Minute = Temp%60;
    Flag = Temp/60;
  }

  // Hour
  if(IncreaseHour || Flag){
    Temp = TimeCurrent->Hour + IncreaseHour  + Flag;
    TimeCurrent->Hour = Temp%24;
    Flag = Temp/24;
  }

  // Day
  if(IncreaseDay || Flag){
    Temp = TimeCurrent->Day + IncreaseDay  + Flag;
    TimeCurrent->Day = Temp%(NumberOfDay+1);
    Flag = Temp/(NumberOfDay+1);
  }

  // Month
  if(IncreaseMonth || Flag){
    Temp = TimeCurrent->Month + IncreaseMonth  + Flag;
    TimeCurrent->Month = Temp%13;
    Flag = Temp/13;
  }

  // Year
  if(IncreaseYear || Flag){
    TimeCurrent->Year += IncreaseYear + Flag;
  }

  CopyMem(TimeFuture, TimeCurrent, sizeof(EFI_TIME));

  return EFI_SUCCESS;
}

/**
  Function for show time information at shell.

  @param[in] HiiFormatStringId   The format string Id for getting from Hii..
  @param[in] Time                The Time information.
 **/
VOID
EFIAPI
  ShowTimeAtShell(
      IN CONST EFI_STRING_ID  HiiFormatStringId,
      IN EFI_TIME  *Time
      )
{
  if(Time == NULL){
    return;
  }
  ShellPrintHiiEx(-1, -1, NULL, HiiFormatStringId, gUefiLoongsonTestHiiHandle,
      Time->Year, Time->Month, Time->Day, Time->Hour, Time->Minute, Time->Second,
      Time->Nanosecond, Time->TimeZone, Time->Daylight);
}

/**
  Function for 'rtcWakeup' command.

  @param[in] ImageHandle  Handle to the Image (NULL if Internal).
  @param[in] SystemTable  Pointer to the System Table (NULL if Internal).
 **/
SHELL_STATUS
  EFIAPI
ShellCommandRunRtcWakeUp (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
    )
{
  EFI_STATUS          Status;
  LIST_ENTRY          *Package;
  CHAR16              *ProblemParam;
  SHELL_STATUS        ShellStatus;
  CONST CHAR16        *ValueStr;
  UINTN               ValInt;
  EFI_TIME            TimeCurrent;
  EFI_TIME            TimeFuture;

  ProblemParam        = NULL;
  ShellStatus         = SHELL_SUCCESS;

  //
  // initialize the shell lib (we must be in non-auto-init...)
  //
  Status = ShellInitialize();
  ASSERT_EFI_ERROR(Status);

  Status = CommandInit();
  ASSERT_EFI_ERROR(Status);

  ASSERT(gRT->GetTime != NULL  && gRT->SetTime != NULL);
  ASSERT(gRT->GetWakeupTime != NULL && gRT->SetWakeupTime != NULL);

  //
  // parse the command line
  //
  Status = ShellCommandLineParse (ParamList, &Package, &ProblemParam, TRUE);

  if (EFI_ERROR(Status)) {
    if (Status == EFI_VOLUME_CORRUPTED && ProblemParam != NULL) {
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_PROBLEM), gUefiLoongsonTestHiiHandle, L"rtcWakeup", ProblemParam);
      FreePool(ProblemParam);
      ShellStatus = SHELL_INVALID_PARAMETER;
    } else {
      ASSERT(FALSE);
    }
  } else {
    Status  = gRT->GetTime(&TimeCurrent, NULL);
    if (EFI_ERROR (Status)) {
      ShellStatus= SHELL_DEVICE_ERROR;
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_ERROR_UNKNOWN), gUefiLoongsonTestHiiHandle, L"GetTime");
      goto GOTO_ERROR;
    }
    ShowTimeAtShell(STRING_TOKEN(STR_GEN_SHOW_CURRENT_TIME), &TimeCurrent);

    // Flag "-h"
    if(ShellCommandLineGetFlag(Package, L"-h") == TRUE){
      if(ShellCommandLineGetCount(Package) == 1){
        if((ValueStr = ShellCommandLineGetValue(Package, L"-h")) != NULL){
          ValInt = ShellStrToUintn (ValueStr);
          Status = IncreaseHourFunc(ValInt, &TimeCurrent, &TimeFuture);
          if(EFI_ERROR(Status)){
            ShellStatus = SHELL_INVALID_PARAMETER;
            ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_ERROR_UNKNOWN), gUefiLoongsonTestHiiHandle, L"IncreaseHourFunc");
            goto GOTO_ERROR;
          }
          Status = gRT->SetWakeupTime(TRUE, &TimeFuture);
          if(EFI_ERROR(Status)){
            ShellStatus = SHELL_INVALID_PARAMETER;
            ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_ERROR_UNKNOWN), gUefiLoongsonTestHiiHandle, L"SetWakeupTime");
            goto GOTO_ERROR;
          }
          goto GOTO_DONE;
        }
        ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gUefiLoongsonTestHiiHandle, L"rtcWakeup" ,L"-h");
        ShellStatus = SHELL_INVALID_PARAMETER;
        goto GOTO_ERROR;
      }
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_MANY), gUefiLoongsonTestHiiHandle, L"rtcWakeup", L"-h");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto GOTO_ERROR;
    }

    // Flag  "-m"
    if(ShellCommandLineGetFlag(Package, L"-m") == TRUE){
      if(ShellCommandLineGetCount(Package) == 1){
        if((ValueStr = ShellCommandLineGetValue(Package, L"-m")) != NULL){
          ValInt = ShellStrToUintn (ValueStr);
          Status = IncreaseMinuteFunc(ValInt, &TimeCurrent, &TimeFuture);
          if(EFI_ERROR(Status)){
            ShellStatus = SHELL_INVALID_PARAMETER;
            ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_ERROR_UNKNOWN), gUefiLoongsonTestHiiHandle, L"IncreaseMinuteFunc");
            goto GOTO_ERROR;
          }
          Status = gRT->SetWakeupTime(TRUE, &TimeFuture);
          if(EFI_ERROR(Status)){
            ShellStatus = SHELL_INVALID_PARAMETER;
            ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_ERROR_UNKNOWN), gUefiLoongsonTestHiiHandle, L"SetWakeupTime");
            goto GOTO_ERROR;
          }
          goto GOTO_DONE;
        }
        ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gUefiLoongsonTestHiiHandle, L"rtcWakeup" ,L"-m");
        ShellStatus = SHELL_INVALID_PARAMETER;
        goto GOTO_ERROR;
      }
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_MANY), gUefiLoongsonTestHiiHandle, L"rtcWakeup", L"-m");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto GOTO_ERROR;
    }

    // Flag "-s"
    if(ShellCommandLineGetFlag(Package, L"-s") == TRUE){
      if(ShellCommandLineGetCount(Package) == 1){
        if((ValueStr = ShellCommandLineGetValue(Package, L"-s")) != NULL){
          ValInt = ShellStrToUintn (ValueStr);
          Status = IncreaseSecondFunc(ValInt, &TimeCurrent, &TimeFuture);
          if(EFI_ERROR(Status)){
            ShellStatus = SHELL_INVALID_PARAMETER;
            ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_ERROR_UNKNOWN), gUefiLoongsonTestHiiHandle, L"IncreaseSecondFunc");
            goto GOTO_ERROR;
          }
          Status = gRT->SetWakeupTime(TRUE, &TimeFuture);
          if(EFI_ERROR(Status)){
            ShellStatus = SHELL_INVALID_PARAMETER;
            ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_ERROR_UNKNOWN), gUefiLoongsonTestHiiHandle, L"SetWakeupTime");
            goto GOTO_ERROR;
          }
          goto GOTO_DONE;
        }
        ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_VALUE), gUefiLoongsonTestHiiHandle, L"rtcWakeup" ,L"-s");
        ShellStatus = SHELL_INVALID_PARAMETER;
        goto GOTO_ERROR;
      }
      ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_TOO_MANY), gUefiLoongsonTestHiiHandle, L"rtcWakeup", L"-s");
      ShellStatus = SHELL_INVALID_PARAMETER;
      goto GOTO_ERROR;
    }

    ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_NO_FIND_FLAG), gUefiLoongsonTestHiiHandle, L"rtcWakeup");
    goto GOTO_ERROR;

GOTO_DONE:
    ShowTimeAtShell(STRING_TOKEN(STR_GEN_SHOW_FUTURE_TIME), &TimeFuture);
    ShellPrintHiiEx(-1, -1, NULL, STRING_TOKEN (STR_GEN_SHUT_DOWN_INFO), gUefiLoongsonTestHiiHandle, DELAY_SHUTDOWN);
    MicroSecondDelay(DELAY_SHUTDOWN*1000*1000);
    gRT->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

GOTO_ERROR:
    //
    // free the command line package
    //
    ShellCommandLineFreeVarList (Package);
  }
  return (ShellStatus);
}
