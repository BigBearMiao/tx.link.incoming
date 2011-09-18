/*
 *  Copyright (C) 2010 Alexander Chernykh 
 *
 *  This file is part of MyQuik: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MyQuik is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *  transactions.h
 */

#pragma once

#include <list>
#include <file.h>
#include "trans2quik.h"
#include "types.h"

enum {
	I_TABLE_ZERO,
	I_TABLE_BIDS,
	I_TABLE_DEALS,
	I_TABLE_STOPS,
};

//
// class Transaction 
//
class Transaction 
{
public:
	enum Flags {
		F_ZERO = 0x00,
		F_PENDING = 0x01,
		F_ACTIVE = 0x08,
		F_SENT = 0x10,
	};
	enum Action { // ��� ����������
		NEW_ORDER, // ����� ������, 
		NEW_NEG_DEAL, // ����� ������ �� ����������� ������, 
		NEW_REPO_NEG_DEAL, // ����� ������ �� ������ ����, 
		NEW_EXT_REPO_NEG_DEAL, // ����� ������ �� ������ ����������������� ���� (����-�), 
		NEW_STOP_ORDER, // ����� ����-������, 
		KILL_ORDER, // ����� ������, 
		KILL_NEG_DEAL, // ����� ������ �� ����������� ������ ��� ������ �� ������ ����, 
		KILL_STOP_ORDER, // ����� ����-������, 
		KILL_ALL_ORDERS, // ����� ��� ������ �� �������� �������, 
		KILL_ALL_STOP_ORDERS, // ����� ��� ����-������, 
		KILL_ALL_NEG_DEALS, // ����� ��� ������ �� ����������� ������ � ������ �� ������ ����, 
		KILL_ALL_FUTURES_ORDERS, // ����� ��� ������ �� ����� FORTS, 
		MOVE_ORDERS, // ����������� ������ �� ����� FORTS, 
		NEW_QUOTE, // ����� ����������� ������, 
		KILL_QUOTE, // ����� ����������� ������, 
		NEW_REPORT, // ����� ����� � ������������� ����������, 
		SET_FUT_LIMIT, // ����� ����������� �� ����������� �����. 
	};

	enum Type { // ��� ������, �������������� ��������
		TYPE_LIMIT,
		TYPE_MARKET,
	};

	enum Operation { // ����������� ������, ������������ ��������
		OPERATION_BUY,
		OPERATION_SELL,
		OPERATION_UNKNOWN = -1,
	};

	enum ExecutionCondition { // ������� ���������� ������, �������������� ��������.
		PUT_IN_QUEUE, // ��������� � ������� (�� ���������), 
		FILL_OR_KILL, // ���������� ��� ���������, 
		KILL_BALANCE, // ����� �������. 
	};

	enum StopOrderKind { // ��� ����-������
		SIMPLE_STOP_ORDER, // ����-�����, 
		CONDITION_PRICE_BY_OTHER_SEC, // � �������� �� ������ ������, 
		WITH_LINKED_LIMIT_ORDER, // �� ��������� �������, 
		TAKE_PROFIT_STOP_ORDER, // ����-������, 
		ACTIVATED_BY_ORDER_SIMPLE_STOP_ORDER, // ����-����� �� ���������� ������, 
		ACTIVATED_BY_ORDER_TAKE_PROFIT_STOP_ORDER, // ����-������ �� ���������� ������. 
	};

	enum StopPriceCondition { // ����������� ����������� ��������� ����-����
		STOPPRICE_CONDITION_LESS,
		STOPPRICE_CONDITION_GREATER,
	};

	enum Expire {
		EXPIRE_TODAY,
		EXPIRE_DATE,
		EXPIRE_GTC,
	};

	enum Status {
		STATUS_SENT, // ���������� ���������� �������, 
		STATUS_OBTAINED, // ���������� �������� �� ������ QUIK �� �������, 
		STATUS_FAILED_CONNECTION, // ������ ��� �������� ���������� � �������� �������, ��������� ����������� ����������� ����� ����, �������� ���������� �� ������������, 
		STATUS_EXECUTED, // ���������� ���������, 
		STATUS_REJECTED, // ���������� �� ��������� �������� ��������, ��� ������ �������� ������� ����� ������ � ���� �DESCRIPTION�, 
		STATUS_WRONG_PARAMETERS, // ���������� �� ������ �������� ������� QUIK �� �����-���� ���������. ��������, �������� �� ������� ���� � ������������ �� �������� ���������� ������� ����, 
		STATUS_WRONG_LIMITS, // ���������� �� ������ �������� ������� ������� QUIK, 
		STATUS_CONFIRMED, // ���������� �������, ����������� � ��������������, ������������ ���������� �����, 
		STATUS_NOT_CONFIRMED, // ���������� �������, ����������� � ��������������, �� ������������ ���������� �����, 
		STATUS_CANCELED, // ���������� �������, ����������� � ��������������, ����� ���������� �����, 
		STATUS_NOT_SUPPORTED, // ���������� �� �������������� �������� ��������. � �������, ������� ��������� �ACTION = MOVE_ORDERS� �� ����, 
		STATUS_WRONG_KEYS, // ���������� �� ������ �������� ������������ ����������� �������. � �������, ���� �����, ������������������ �� �������, �� ������������� ������� ������������ ����������.

		STATUS_EX_UNKNOWN,
		STATUS_EX_BID_ACTIVE, // ������ �������;
		STATUS_EX_BID_EXECUTED, // ���������;
		STATUS_EX_BID_CANCELED, // �����;
	};

	struct Answer {
		long result;
		long extendedErrorCode;
		long replyCode;
		DWORD id;
		double orderNum;
		char replyMessage[1024];
		struct Ext {
			int callback;
			int line;
		} ext;

		Answer()
		{}
		Answer(long nTransactionResult, long nTransactionExtendedErrorCode, long nTransactionReplyCode, 
			DWORD dwTransId, double dOrderNum, LPCSTR lpcstrTransactionReplyMessage, int line = 0, int callback = 0);
	};

	Transaction();

	static LPCTSTR ActionToString(Transaction::Action action);
	static LPCTSTR ActionToStringRus(Transaction::Action action);
	static LPCTSTR StatusToString(Transaction::Status status);

	static BOOL IsNewOrder(Transaction::Action action);
	static BOOL IsActionCancel(Transaction::Action action);
	static BOOL IsActionStop(Transaction::Action action);
	static BOOL IsActive(Transaction::Status status);
	static BOOL IsExecuted(Transaction::Status status);
	static BOOL IsCanceled(Transaction::Status status);
	static BOOL IsFailed(Transaction::Status status);

	static DWORD LineToTableIndex(int line) { return HIWORD(line); }
	static DWORD LineToLineIndex(int line) { return LOWORD(line); }

	static DWORD MakeLine(int table, int line) { return MAKELONG(line, table); }

	void SetPending(BOOL set = TRUE);

	DWORD id;
#if 1
	DWORD line; // HIWORD(table) | LOWORD(line)
#endif
	Status status;
	SYSTEMTIME time;

	TCHAR strName[128];
	TCHAR strClassCode[32];
	TCHAR strSecCode[32];
	TCHAR strAccount[32];
	TCHAR strClientCode[32];
	Action action;
	Action baseAction;
	Type type;
	BOOL marketMakerOrder;
	Operation operation;
	ExecutionCondition executionCondition;
	double quantity;
	double price;
	double volume;
	struct Stop {
		double price;
		StopOrderKind orderKind;
		TCHAR strClassCode[32];
		TCHAR strSecCode[32];
		StopPriceCondition condition;
		double linkedOrderPrice;
	} stop;
	struct _Expire {
		Expire type;
		SYSTEMTIME date;
	} expire;		
	double orderKey;
	double stopOrderKey;

	DWORD flags;
#if 0
	TCHAR strReplyMessage[1024];
#endif
	struct Next {
		Action action;
		Operation operation;
		double price;
		double quantity;
		int flags;
	} next;
}; // class Transaction

//
// class Transactions 
//
typedef std::list<Transaction*> ListOfTransactions;
typedef std::list<ListOfTransactions*> ListOfDayTransactions;

class Transactions : public ListOfDayTransactions
{
public:
	struct Iterator {
		ListOfDayTransactions::iterator list;
		ListOfTransactions::iterator ta;
	};

	Transactions();
	~Transactions();

	int GetNrItems() const;

	Transaction * Create(Transaction * pTransaction, LPCTSTR name, WORD id, const SYSTEMTIME * pTime);
	void Clear();

	int Add(Transaction * pTa, Transactions::Iterator * pIts = NULL);
	int AddCopy(Transaction * pTa);
	int Remove(Transactions::Iterator its);

	Transaction * FindTransaction(DWORD id, double num, DWORD line, DWORD dwDate = 0);
	Transaction * FindTransactionById(DWORD id);
	Transaction * FindTransactionByOrderNumberEx(double num, DWORD line = 0, DWORD dwDate = 0, DWORD flags = 0x0);
	Transaction * FindTransactionByOrderNumber(double num, DWORD line = 0, DWORD dwDate = 0);
	Transaction * FindTransactionByOrderNumber2(double num, DWORD line, DWORD dwDate = 0);
	Transaction * FindTransactionByOrderNumber2(double num, DWORD line, Transactions::Iterator & its, DWORD dwDate = 0);
	Transaction * FindTransactionByDate(SYSTEMTIME & stime, Transactions::Iterator & its);
	Transaction * FindTransactionByDate(SYSTEMTIME & stime1, SYSTEMTIME & stime2, Transactions::Iterator & its);
	Transaction * FindTransactionByDate(DWORD date, Transactions::Iterator & its);

	Transaction * FindFirstTransaction(Transactions::Iterator & its);
	Transaction * FindNextTransaction(Transactions::Iterator & its, BOOL autoIncList = TRUE);

	Transaction * FindFirstDate(Transactions::Iterator & its);
	Transaction * FindNextDate(Transactions::Iterator & its);

	Transaction * FindSent(double num, DWORD dwDate = 0);

	int GetDates(::std::list<DWORD> * pDates);
}; // class Transactions 

#define COMPARE_DWDATES_AND_CONTINUE(date) \
	if (date != 0) \
	{ \
		DWORD d = GetTimeStampDate(pList->front()); \
		if (d != (date)) \
			continue; \
	}

//
// class TransCtrl 
//
class TransCtrl 
{
public:
	enum {
		F_DELETE = 0x1,
		F_CANCEL = 0x2,
	};

public:	
	TransCtrl();
	~TransCtrl();

	int Init(HWND hWnd, int autoCheck = TRUE);
	void Close();

	void DeleteAllTransactions(int flags = 0);

	int SetAutocheck(int autoCheck = TRUE);
	int CheckConnection();

	Transaction * CreateNewTransaction(Transaction * pTransaction, LPCTSTR name, int updateParams = 1, Transactions::Iterator * pIts = NULL);
	Transaction * UpdateTransaction(Transaction * pTransaction, int updateParams);
	int  RemoveTransaction(const Transaction * pTransaction, Transactions::Iterator & its);

	int DoTransaction(Transaction * pTransaction, int nd, int flags = 0x0, CWindow * pWnd = NULL, int * pResult = NULL);
	int CancelTransactions(Transaction::Action action, Transaction::Operation operation, 
		LPCTSTR seccode, LPCTSTR classcode, LPCTSTR clientcode, LPCTSTR account, CWindow * pWnd = NULL, int * pResult = NULL);

	Transaction * FindTransaction(DWORD id, double num, DWORD line, DWORD dwDate = 0);
	Transaction * FindTransactionById(DWORD id);
	Transaction * FindTransactionByOrderNumberEx(double num, DWORD line = 0, DWORD dwDate = 0, DWORD flags = 0x0);
	Transaction * FindTransactionByOrderNumber(double num, DWORD line = 0, DWORD dwDate = 0);
	Transaction * FindTransactionByOrderNumber2(double num, DWORD line, Transactions::Iterator & its, DWORD dwDate = 0);
	Transaction * FindSent(double num, DWORD dwDate = 0);

	int GetDates(::std::list<DWORD> * pDates);

	Transaction * OnAnswer(Transaction::Answer * pAnswer, Transaction * pTransaction);

public:
	Transactions & GetTransactions() { return m_transactions; }

protected:
		int SendString(LPTSTR str, size_t size, int n, int flags, CWindow * pWnd, int * pResult);

protected:
	BOOL m_initialized;

	TRANS2QUIK m_trans2quik;
	WORD m_transId;
	struct Tx {
		WORD n;
	} m_tx;

	HWND m_hWnd;
	Transactions m_transactions;
}; // class TransCtrl 


#define SET_TRANSACTION_STRCODE(dst, src) SAFE_TSTRCPY(dst, src)

int COMPARE_TRANSACTIONS(const Transaction * pTa1, const Transaction * pTa2);


enum {
	TIMESTAMP_TIME = 0x1,
	TIMESTAMP_DATE = 0x2,
};

#define MAKE_TIME_STAMP_TIME(h, m, s) (DWORD)(((s) & 0x3f) | (((m) & 0x3f) << 6) | (((h) & 0x1f) << 12))
#define MAKE_TIME_STAMP_DATE(Y, M, D) (DWORD)(((D) & 0x1f) | (((M) & 0x0f) << 5) | (((Y) & 0x3fff) << 9))

QWORD GetTimeStamp64(const SYSTEMTIME & stime, int flags = TIMESTAMP_TIME|TIMESTAMP_DATE);
DWORD GetTimeStampTime(const SYSTEMTIME & stime);
DWORD GetTimeStampDate(const SYSTEMTIME & stime);

QWORD GetTimeStamp64(const Transaction * pTa, int flags = TIMESTAMP_TIME|TIMESTAMP_DATE);
DWORD GetTimeStampTime(const Transaction * pTa);
DWORD GetTimeStampDate(const Transaction * pTa);

int GetYearFromTimeStamp(DWORD date);
int GetMonthFromTimeStamp(DWORD date);
int GetDayFromTimeStamp(DWORD date);

int Write(const Transaction * pTa, my::lib::File & file);
int Read(Transaction & ta, my::lib::File & file);

enum {
	F_PRINT_DATE = 0x1,
	F_PRINT_TIME = 0x2,
};
int MakeDescription(const Transaction * pTa, LPTSTR text, size_t size, int nd);
int StrPrintTransactionParameters(const Transaction * pTa, LPTSTR text, size_t size, int nd = 0, int flags = F_PRINT_DATE|F_PRINT_TIME);
