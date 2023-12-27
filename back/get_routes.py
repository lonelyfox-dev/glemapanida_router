import requests
import json
import argparse

api_key1 = "cc5974a7-6c92-49a7-96f8-b7ca5881e3c3"

def process_stations(request_data):
    try:
        # Получаем данные из запроса
        station_code1 = request_data['station_code1']
        station_code2 = request_data['station_code2']
        date = request_data['date']

        URL_API = "https://api.rasp.yandex.net/v3.0/search/?apikey=" + api_key1 + "&format=json&from=" + station_code1 + "&to=" + station_code2 + "&lang=ru_RU&page=1&date=" + date

        # Отправляем запрос на сторонний API с кодами станций
        api_response = requests.get(URL_API)

        # Проверяем успешность запроса к стороннему API
        api_response.raise_for_status()

        # Получаем данные из ответа стороннего API
        api_data = api_response.json()

        with open('simple_routes.json', 'w', encoding='utf-8') as json_file:
            json.dump(api_data, json_file, ensure_ascii=False, indent=4)

        # Возвращаем данные в ответе
        return json.dumps(api_data, ensure_ascii=False, indent=4)

    except Exception as e:
        return json.dumps({"error": str(e)}, ensure_ascii=False, indent=4)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Get routes information.")
    parser.add_argument("station_code1", type=str, help="Code of the departure station")
    parser.add_argument("station_code2", type=str, help="Code of the destination station")
    parser.add_argument("date", type=str, help="Date of the travel in the format YYYY-MM-DD")
    args = parser.parse_args()

    request_data = {
        "station_code1": args.station_code1,
        "station_code2": args.station_code2,
        "date": args.date
    }

    result = process_stations(request_data)
    print(result)
