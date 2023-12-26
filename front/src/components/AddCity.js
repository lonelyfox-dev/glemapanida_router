// import React from "react";

// class AddCity extends React.Component{
//     render(){
//         return (
//             <form>
//                 <input placeholder="Введите город"/>
//                 <button type="button">Добавить</button>
//             </form>
//         )
//     }
// }
// export default AddCity
import React from "react";
class AddCity extends React.Component {
  
    state = {
      cities: []
    }
  
    handleInputChange = (e) => {
      const cityName = e.target.value;
      fetch(`http://localhost:8080/getCity?search=${cityName}`)
        .then(response => response.json())
        .then(data => {
          this.setState({ cities: data });
        })
        .catch(error => console.error(error));
    }
    handleAddClick = () => {
      if (this.state.cities.length === 1) {
        this.props.onAdd(this.state.cities[0]);
      }
    }
    render() {
      return (
        <form>
          <input list="cities" placeholder="Введите город" onChange={this.handleInputChange} />
          <datalist id="cities" style={{ width: '100%' }}>
            {this.state.cities.map(city => (
              <option key={city.name} value={city.name} />
            ))}
          </datalist>
          <button type="button" onClick={this.handleAddClick}>Добавить</button>
        </form>
      )
    }
  }
  export default AddCity