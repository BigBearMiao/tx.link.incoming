namespace SampleRealTimeTesting
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

		public SmaStrategy(CandleManager candleManager, Ma longMa, Ma shortMa, TimeSpan timeFrame)
			: base(timeFrame)
		{
			_candleManager = candleManager;

			this.LongMa = longMa;
			this.ShortMa = shortMa;
		}

		public Ma LongMa { get; private set; }
		public Ma ShortMa { get; private set; }

		protected override void OnStarting()
		{
			// ���������� ������� ��������� ������������ ���� �����
			_isShortLessThenLong = this.ShortMa.Value < this.LongMa.Value;

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
			this.LongMa.Add(candle);
			this.ShortMa.Add(candle);

			// ��������� ����� ��������� ������������ ���� �����
			var isShortLessThenLong = this.ShortMa.Value < this.LongMa.Value;

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