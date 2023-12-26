import React from 'react';
import '../css/TicketButton.css'; // Подключаем файл стилей

class TicketButton extends React.Component {
  constructor(props) {
    super(props);
    this.state = { tickets: [] }; // Используем массив для хранения билетов
    this.cities = props.cities;
    this.transportState=props.transportState
  }

  handleGetTicket = () => {
    const { cities, transportState } = this.props; // Получаем словари из props

  // Преобразуем словарь cities в формат параметров запроса
  const cityParams = Object.keys(cities)
  .map((key, index) => `city${index + 1}=${cities[key].name}`)
  .join('&');
  console.log(cityParams)

  const transportParams = Object.keys(transportState)
  .map(key => `${key}=${transportState[key]}`)
  .join('&');

  const url = `http://localhost:8080/getTicket?${cityParams}&${transportParams}`;

  console.log(transportParams)

    fetch(url)
      .then(response => response.json())
      .then(data => {
        console.log(data);
        console.log(this.transportState);
        this.setState({ tickets: data }); // Обновляем состояние массивом билетов из JSON
      })
      .catch(error => console.error(error));
  }

  render() {
    return (
      <div className="ticket-container">
        <main style={{ position: "flex" }}>
        <div style={{ position: "absolute", zIndex: 1, left:0 }}>
        <CustomButton onClick={this.handleGetTicket} />
        </div>
        <div style={{ position: "flex", zIndex: 2, left:0}}>
        {this.state.tickets.map((ticket, index) => (
          <div className="ticket" key={index}>
            <div className="rounded-box">
              {Object.keys(ticket).map(key => (
                <p key={key}>{key}: {ticket[key]}</p>
              ))}
            </div>
          </div>
        ))}
        </div>
        </main>
      </div>
    );
  }
}

class CustomButton extends React.Component {
  render() {
    return (
      <button style={{ float: 'left', position: "absolute" }} className="custom-button" onClick={this.props.onClick}>Маршрут</button>
    );
  }
}

export default TicketButton;

