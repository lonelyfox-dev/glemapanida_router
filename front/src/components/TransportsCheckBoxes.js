import React from 'react';
import '../css/CheckboxWithImages.css';
import image1 from '../images/1.svg';
import image2 from '../images/2.svg';
import image3 from '../images/3.svg';

const TransportsCheckBoxes = ({ transportState, onCheckboxChange }) => {
  const handleCheckboxChange = (e) => {
    const { name } = e.target;
    onCheckboxChange(name); // вызываем обработчик изменения чекбоксов
}
    return (
      <div className="transports-check-boxes">
        <input type="checkbox" id="image1" name="plane" checked={transportState.plane} onChange={handleCheckboxChange}/>
        <label htmlFor="image1">
          <img src={image1} alt="Image 1" width="50" height="50" />
        </label>
  
        <input type="checkbox" id="image2" name="bus" checked={transportState.bus} onChange={handleCheckboxChange} />
        <label htmlFor="image2">
          <img src={image2} alt="Image 2" width="50" height="50" />
        </label>
  
        <input type="checkbox" id="image3" name="train" checked={transportState.train} onChange={handleCheckboxChange} />
        <label htmlFor="image3">
          <img src={image3} alt="Image 3" width="50" height="50" />
        </label>
      </div>
    );
  };
  
  export default TransportsCheckBoxes;