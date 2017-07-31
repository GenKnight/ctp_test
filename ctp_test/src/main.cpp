#include <iostream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include "CustomMdSpi.h"
#include "CustomTradeSpi.h"
#include "TickToKlineHelper.h"

using namespace std;

//#define  MD

// ---- ȫ�ֱ��� ---- //
// ��������
TThostFtdcBrokerIDType gBrokerID = "9999";                              // ģ�⾭���̴���
TThostFtdcInvestorIDType gInvesterID = "087652";                        // Ͷ�����˻���
TThostFtdcPasswordType gInvesterPassword = "963852";                    // Ͷ��������
int requestID = 0; // ������

// �������
char md_con_dir[] = "./md_con/";
CThostFtdcMdApi *g_pMdUserApi = nullptr;                           // ����ָ��
char gMdFrontAddr[] = "tcp://180.168.146.187:10010";               // ģ������ǰ�õ�ַ
//char *g_pInstrumentID[] = {"TF1712", "ag1712", "m1712", "SR811"};  // �����Լ�����б��С��ϡ���֣��������ѡһ��
//int instrumentNum = 4;                                             // �����Լ��������
char *g_pInstrumentID[] = { "ag1712" };                            // �����Լ�����б��С��ϡ���֣��������ѡһ��
int instrumentNum = 1;                                             // �����Լ��������
unordered_map<string, TickToKlineHelper> g_KlineHash;              // ��ͬ��Լ��k�ߴ洢��
std::mutex marketDataMutex;

// ���ײ���
char tr_con_dir[] = "./tr_con/";
CThostFtdcTraderApi *g_pTradeUserApi = nullptr;                    // ����ָ��
char gTradeFrontAddr[] = "tcp://180.168.146.187:10030";            // ģ�⽻��ǰ�õ�ַ
TThostFtdcInstrumentIDType g_pTradeInstrumentID = "m1709";         // �����׵ĺ�Լ����

int main()
{
#ifdef MD
	// ��ʼ�������߳�
	cout << "��ʼ������..." << endl;
    g_pMdUserApi = CThostFtdcMdApi::CreateFtdcMdApi(md_con_dir);   // ��������ʵ��
	CThostFtdcMdSpi *pMdUserSpi = new CustomMdSpi();       // ��������ص�ʵ��
	g_pMdUserApi->RegisterSpi(pMdUserSpi);               // ע���¼���
	g_pMdUserApi->RegisterFront(gMdFrontAddr);           // ��������ǰ�õ�ַ
	g_pMdUserApi->Init();                                // ��������
#endif	


	// ��ʼ�������߳�
	cout << "��ʼ������..." << endl;
    g_pTradeUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi(tr_con_dir); // ��������ʵ��
	CThostFtdcTraderSpi *pTradeSpi = new CustomTradeSpi();
	//CustomTradeSpi *pTradeSpi = new CustomTradeSpi;                 // �������׻ص�ʵ��
	g_pTradeUserApi->RegisterSpi(pTradeSpi);                        // ע���¼���
    g_pTradeUserApi->RegisterFront(gTradeFrontAddr);                // ���ý���ǰ�õ�ַ
	g_pTradeUserApi->SubscribePublicTopic(THOST_TERT_RESTART);      // ���Ĺ�����
    g_pTradeUserApi->SubscribePrivateTopic(THOST_TERT_RESUME);     // ����˽����
	g_pTradeUserApi->Init();                                        // ��������
		
#ifdef MD
	// �ȵ��߳��˳�
	g_pMdUserApi->Join();
	delete pMdUserSpi;
	g_pMdUserApi->Release();
#endif
	g_pTradeUserApi->Join();
	delete pTradeSpi;
	g_pTradeUserApi->Release();

	// ת������k������
	//TickToKlineHelper tickToKlineHelper;
	//tickToKlineHelper.KLineFromLocalData("market_data.csv", "K_line_data.csv");
	
	getchar();
	return 0;
}