#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "TickToKlineHelper.h"
#include "StrategyTrade.h"
#include "CustomTradeSpi.h"

extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash;
// �̻߳�����
extern std::mutex marketDataMutex;

void StrategyCheckAndTrade(TThostFtdcInstrumentIDType instrumentID, CustomTradeSpi *customTradeSpi)
{
	// ����
	std::lock_guard<std::mutex> lk(marketDataMutex);
    auto it = g_KlineHash.find(instrumentID);
    if (it != g_KlineHash.end()) {
        // ����
        std::vector<double> priceVec = it->second.m_priceVec;
        if (priceVec.size() >= 3) {
            int len = priceVec.size();
            // ��������������Ǿ��򿪲�,��֮��������,������ʱ�����һ���۸��µ�
            if (priceVec[len - 1] > priceVec[len - 2] && priceVec[len - 2] > priceVec[len - 3])
                customTradeSpi->reqOrderInsert(instrumentID, priceVec[len - 1], 1, THOST_FTDC_D_Buy);
            else if (priceVec[len - 1] < priceVec[len - 2] && priceVec[len - 2] < priceVec[len - 3])
                customTradeSpi->reqOrderInsert(instrumentID, priceVec[len - 1], 1, THOST_FTDC_D_Buy);
        }
    }

}
