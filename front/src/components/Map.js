import React, { Component } from 'react';
import ymaps from 'ymaps';
class Map extends Component {
  componentDidMount() {
    // Грузим карту из скрипта
    const script = document.createElement('script');
    script.src = "https://api-maps.yandex.ru/2.1/?lang=ru_RU&amp;apikey=d472c797-6f87-441f-965f-cd4d24327566";
    script.async = true;

    // Добавляем обработчик загрузки скрипта
  
    script.onload = () => {
      // В этом месте ymaps уже будет доступна, так как скрипт загружен

      ymaps
      .load()
      .then(maps => {
        const map = new maps.Map('map', {
          center: [55.76, 37.64],
          zoom: 6,
        }, 
        );
      })
      .catch(error => console.log('Failed to load Yandex Maps', error));
    };

    document.body.appendChild(script);
  }

  render() {
    return <div id="map" style={{ width: '60%', height: '500px', marginLeft: 'auto',marginTop:'auto'}} />;
  }
}

export default Map;