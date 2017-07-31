# ctp_test
用ctp官方接口写的一个简单demo，具备行情、交易、k线数据以及策略功能，可以连接simnow模拟交易~
编译环境是windows+vs2013，如果要用在linux上，需要用linux版的ctp api

# 数据字典
order_status
///全部成交
#define THOST_FTDC_OST_AllTraded '0'
///部分成交还在队列中
#define THOST_FTDC_OST_PartTradedQueueing '1'
///部分成交不在队列中
#define THOST_FTDC_OST_PartTradedNotQueueing '2'
///未成交还在队列中
#define THOST_FTDC_OST_NoTradeQueueing '3'
///未成交不在队列中
#define THOST_FTDC_OST_NoTradeNotQueueing '4'
///撤单
#define THOST_FTDC_OST_Canceled '5'
///未知
#define THOST_FTDC_OST_Unknown 'a'
///尚未触发
#define THOST_FTDC_OST_NotTouched 'b'
///已触发
#define THOST_FTDC_OST_Touched 'c'

FrontID + SessionID + OrderRef
登陆之后，交易核心会返回对应此次连接的前置机编号FrontID 和会话编号SessionID。这两个编号在此次连
接中是不变的。
OrderRef 是报单操作的核心数据结构CThostFtdcInputOrderField 中的一个字段。开发者可以让OrderRef 在一
次登录期间从MaxOrderRef 起逐一递增，以保证报单的唯一性。开发者也可以选择不对它赋值，则交易核心
会自动赋予一个唯一性的值。

ExchangeID + TraderID + OrderLocalID
交易核心将报单提交到报盘管理之后由交易核心生成OrderLocalID 并返回给客户端的。ExchangeID 合约所在
交易所的代码，TraderID 由交易核心选定返回。客户端也可以通过这组序列号进行撤单操作。
与第一组序列号不同的是：该序列号是由综合交易平台的交易核心维护。

ExchangeID + OrderSysID
交易所在接收了报单之后，会为该报单生成报单在交易所的编号OrderSysID。再经由综合交易平台转发给客
户端。ExchangeID 是固定的。
客户端也可以通过这组序列号进行撤单操作。
这组序列号由交易所维护。

交易所返回
委托单id
OrderSysID
成交单id
TradeID
