#!/usr/bin/env python3
import sys
import pandas as pd
from prophet import Prophet

def predict_trends(file_path, months):
    # Read the data from the CSV file
    data = pd.read_csv(file_path)
    data['Date'] = pd.to_datetime(data['Date'])

    # Create a copy of the original CSV file
    data_copy = '/tmp/stock-price-copy.csv'
    data.to_csv(data_copy, index=False)

    # Initialize the output dataframe for predictions
    forecast_df = pd.DataFrame()

    for column in ['Open', 'High', 'Low', 'Close', 'Adj Close', 'Volume']:
        # Prep the data for prophet
        df = data[['Date', column]].rename(columns={'Date': 'ds', column: 'y'})

        # Initialize the prophet model
        model = Prophet(yearly_seasonality=True,
                        daily_seasonality=False,
                        seasonality_mode='additive',
                        seasonality_prior_scale=10.0,
                        changepoint_prior_scale=0.1)

        # Fit the model
        model.fit(df)

        # Create a future dataframe
        future = model.make_future_dataframe(periods=months * 30)
        forecast = model.predict(future)

        # Smooth the forecast
        forecast['yhat'] = forecast['yhat'].rolling(window=5, min_periods=1).mean()

        # Merge the forecast with original data
        forecast = forecast[['ds' 'yhat']].rename(columns={'ds': 'Date', 'yhat': column})
        forecast = forecast[forecast['Date'] > df['ds'].max()]
        if column == 'Volume':
            forecast[column] = forecast[column].round().astype(int)
        else:
            forecast[column] = forecast[column].round(6)

        if forecast_df.empty:
            forecast_df = forecast
        else:
            forecast_df = forecast_df.merge(forecast, on='Date')

        # Combine the original data with the forecast data
        combined_data = pd.concat([data, forecast_df], ignore_index=True)

        # Save the combined data to /tmp/predictions.csv
        combined_data.to_csv('/tmp/predictions.csv', index=False)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python predict_stock.py <path_to_csv> <months>")
    else:
        file_path = sys.argv[1]
        months = int(sys.argv[2])
        predict_trends(file_path, months)
