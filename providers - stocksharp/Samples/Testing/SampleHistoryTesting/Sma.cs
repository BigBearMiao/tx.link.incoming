namespace SampleHistoryTesting
{
	using System;
	using System.Collections.Generic;

	using StockSharp.Algo.Candles;

	/// <summary>
	/// ���������� ���������� ���������� �������.
	/// </summary>
	class Sma
	{
		private readonly List<TimeFrameCandle> _buffer = new List<TimeFrameCandle>();

		/// <summary>
		/// ������� ����������� �������.
		/// </summary>
		/// <param name="length">����� ����������.</param>
		public Sma(int length)
		{
			this.Length = length;
		}

		/// <summary>
		/// ����� ����������.
		/// </summary>
		public int Length { get; private set; }

		/// <summary>
		/// ������� ��������.
		/// </summary>
		public decimal Value { get; private set; }

		/// <summary>
		/// ������ ������. ����� ������������ ���������� ������� ����� ����� <see cref="Add"/>
		/// ����� ������ ������ <see cref="Length"/>.
		/// </summary>
		public int BufferSize { get { return _buffer.Count; } }

		/// <summary>
		/// �������� ����� ������.
		/// </summary>
		/// <param name="candle">����� ������.</param>
		public void Add(TimeFrameCandle candle)
		{
			if (candle == null)
				throw new ArgumentNullException("candle");

			var value = this.Value;

			// ��������� ����� ������
			value += candle.ClosePrice / this.Length;
			_buffer.Add(candle);

			// ���� ������ ���� ���������� ������� (���� ������ �����)
			if (_buffer.Count > this.Length)
			{
				// ������� ��������� ��������
				value -= _buffer[0].ClosePrice / this.Length;
				_buffer.RemoveAt(0);
			}

			this.Value = value;
		}
	}
}