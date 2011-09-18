namespace SampleRealTimeTesting
{
	using System;

	using StockSharp.Algo.Candles;

	/// <summary>
	/// ���������� ���������� ������� ���������� �������.
	/// </summary>
	class Sma : Ma
	{
		/// <summary>
		/// ������� ����������� �������.
		/// </summary>
		/// <param name="length">����� ����������.</param>
		public Sma(int length)
			: base(length)
		{
		}

		/// <summary>
		/// �������� ����� ������.
		/// </summary>
		/// <param name="candle">����� ������.</param>
		public override void Add(TimeFrameCandle candle)
		{
			if (candle == null)
				throw new ArgumentNullException("candle");

			var value = base.Value;

			// ��������� ����� ������
			value += candle.ClosePrice / this.Length;
			base.Buffer.Add(candle);

			// ���� ������ ���� ���������� ������� (���� ������ �����)
			if (base.BufferSize > base.Length)
			{
				// ������� ��������� ��������
				value -= base.Buffer[0].ClosePrice / base.Length;
				base.Buffer.RemoveAt(0);
			}

			base.Value = value;
		}
	}
}