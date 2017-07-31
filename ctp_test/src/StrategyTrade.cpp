#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "TickToKlineHelper.h"
#include "StrategyTrade.h"
#include "CustomTradeSpi.h"

extern std::unordered_map<std::string, TickToKlineHelper> g_KlineHash;
// 线程互斥量
extern std::mutex marketDataMutex;

void StrategyCheckAndTrade(TThostFtdcInstrumentIDType instrumentID, CustomTradeSpi *customTradeSpi)
{
	// 加锁
	std::lock_guard<std::mutex> lk(marketDataMutex);
    auto it = g_KlineHash.find(instrumentID);
    if (it != g_KlineHash.end()) {
        // 策略
        std::vector<double> priceVec = it->second.m_priceVec;
        if (priceVec.size() >= 3) {
            int len = priceVec.size();
            // 最后连续三个上涨就买开仓,反之就卖开仓,这里暂时用最后一个价格下单
            if (priceVec[len - 1] > priceVec[len - 2] && priceVec[len - 2] > priceVec[len - 3])
                customTradeSpi->reqOrderInsert(instrumentID, priceVec[len - 1], 1, THOST_FTDC_D_Buy);
            else if (priceVec[len - 1] < priceVec[len - 2] && priceVec[len - 2] < priceVec[len - 3])
                customTradeSpi->reqOrderInsert(instrumentID, priceVec[len - 1], 1, THOST_FTDC_D_Buy);
        }
    }

}
