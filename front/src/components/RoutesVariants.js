import React from 'react';
import '../css/CheckboxWithImages.css';
import moneyImg from '../images/money.svg';
import timeImg from '../images/time.svg';

const RoutesVariant = ({ routeState, onCheckboxChange }) => {
  const handleCheckboxChange = (e) => {
    const { name } = e.target;
    onCheckboxChange(name); // вызываем обработчик изменения чекбоксов
}
    return (
      <div className="transports-check-boxes">
        <input type="checkbox" id="moneyImg" name="money" checked={routeState.money} onChange={handleCheckboxChange}/>
        <label htmlFor="moneyImg">
          <img src={moneyImg} alt="money Img" width="40" height="40" />
        </label>
        <input type="checkbox" id="timeImg" name="travelTime" checked={routeState.travelTime} onChange={handleCheckboxChange}/>
        <label htmlFor="timeImg">
          <img src={timeImg} alt="time Img" width="40" height="40" />
        </label>
      </div>
    );
  };
  
  export default RoutesVariant;