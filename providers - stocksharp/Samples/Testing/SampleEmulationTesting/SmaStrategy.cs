namespace SampleEmulationTesting
{
	using System;

	using StockSharp.Algo;
	using StockSharp.Algo.Candles;
	using StockSharp.Algo.Strategies;
	using StockSharp.BusinessEntities;

	class SmaStrategy : TimeFrameStrategy
	{
		private readonly CandleManager _candleManager;
		private bool _isShortLessThenLong;

		private DateTime _nextTime;

		public SmaStrategy(CandleManager candleManager, Sma longSma, Sma shortSma, TimeSpan timeFrame)
			: base(timeFrame)
		{
			_candleManager = candleManager;

			this.LongSma = longSma;
			this.ShortSma = shortSma;
		}

		public Sma LongSma { get; private set; }
		public Sma ShortSma { get; private set; }

		protected override void OnStarting()
		{
			// ���������� ������� ��������� ������������ ���� �����
			_isShortLessThenLong = this.ShortSma.Value < this.LongSma.Value;

			// ��������� ����� ��������� ������� �����������
			_nextTime = base.TimeFrame.GetCandleBounds(base.Trader).Max;

			base.OnStarting();
		}

		protected override StrategyProcessResults OnProcess()
		{
			// ���� ���� ��������� � �������� ���������
			if (base.ProcessState == StrategyProcessStates.Stopping)
			{
				// �������� �������� ������
				base.CancelActiveOrders();

				// ��� ��� ��� �������� ������ �������������� ���� ��������, �� ���������� StrategyProcessResults.Stop
				return StrategyProcessResults.Stop;
			}

			// ������� ��������� �������� ��������� ��������� ������� ���, ��� ������, ��� ��������� ������� 5-�������.
			if (base.Trader.MarketTime < _nextTime)
			{
				// ���������� StrategyProcessResults.Continue, ��� ��� ��� �������� ��� �� �������� ���� ������, � ������ ������� ���������� ������.
				return StrategyProcessResults.Continue;
			}

			// �������� �������������� ������
			var candle = _candleManager.GetTimeFrameCandle(base.Security, base.TimeFrame, _nextTime - base.TimeFrame);

			// ���� ������ �� ���������� (�� ���� �� ����� ������ � ����-������), �� ���� ��������� ��������� ������.
			if (candle == null)
			{
				// ���� ������ ������ 10 ������ � ������� ��������� ������, � ��� ��� � �� ���������,
				// ������ ������ � ��������� 5-������� �� ����, � ��������� �� ��������� ������
				if ((base.Trader.MarketTime - _nextTime) > TimeSpan.FromSeconds(10))
					_nextTime += base.TimeFrame;

				return StrategyProcessResults.Continue;
			}

			_nextTime += base.TimeFrame;

			// ��������� ����� ������
			this.LongSma.Add(candle);
			this.ShortSma.Add(candle);

			// ��������� ����� ��������� ������������ ���� �����
			var isShortLessThenLong = this.ShortSma.Value < this.LongSma.Value;

			// ���� ��������� �����������
			if (_isShortLessThenLong != isShortLessThenLong)
			{
				// ���� �������� ������ ��� �������, �� �������, �����, �������.
				var direction = isShortLessThenLong ? OrderDirections.Sell : OrderDirections.Buy;

				// ������� ������
				var order = base.CreateOrder(direction, base.Security.GetMarketPrice(direction), base.Volume);

				// ������������ ������ (������� �������� - �������������� �������)
				// base.RegisterOrder(order);

				// ������������ ������ (����� �����������)
				var strategy = new MarketQuotingStrategy(order, new Unit(), new Unit());
				base.ChildStrategies.Add(strategy);

				// ���������� ������� ��������� ������������ ���� �����
				_isShortLessThenLong = isShortLessThenLong;
			}

			return StrategyProcessResults.Continue;
		}
	}
}