import React from "react"
import Header from "./components/Header"
import AddCity from "./components/AddCity"
import Cities from "./components/Cities"
import TicketButton from "./components/TicketButton"
import TransportsCheckBoxes from "./components/TransportsCheckBoxes"
import RoutesVariant from "./components/RoutesVariants"
import './css/main.css';
class App extends React.Component{
    constructor(props){
        super(props)
        this.state={
            cities:[

            ],
            transportState: {
                plane: true,
                bus: true,
                train: true
            },
            routeState:{
                money:true,
                travelTime:false
            }
        }
        this.addCity=this.addCity.bind(this)
    }
    
    render(){
        return(<div>
            <Header title="Glemapanida"/>
            <aside>
                <AddCity onAdd={this.addCity}/>
                <RoutesVariant
                routeState={this.state.routeState}
                onCheckboxChange={this.handleCheckboxChangeRoute}
                />
                <TransportsCheckBoxes  
                    transportState={this.state.transportState}
                    onCheckboxChange={this.handleCheckboxChange}
                />
                
                <main style={{ position: "flex"}}>
                    <div style={{ position: "absolute", zIndex: 1 }}>
                        <Cities cities={this.state.cities}/>    
                    </div>
                    <div style={{ position: "relative", zIndex: 2}}>
                        <TicketButton cities={this.state.cities}  transportState={this.state.transportState} routeState={this.state.routeState}/>            
                    </div>
                </main>
            
            </aside>
        </div>)
    }
    addCity(city){
        const existingCityIndex = this.state.cities.findIndex(c => c.name === city.name);
        if (existingCityIndex != -1) {
            return
        }
        const id = this.state.cities.length+1
        this.setState({cities:[...this.state.cities,{id,...city}]})
        console.log(city)
        console.log(this.state.cities)
    }

    handleCheckboxChange = (name) => {
        console.log(this.state.transportState)
        this.setState({
            transportState: {
                ...this.state.transportState,
                [name]: !this.state.transportState[name] // инвертируем значение чекбокса
            }
        });
    }
    handleCheckboxChangeRoute = (name) => {
        console.log(this.state.routeState)
        console.log(name)
        if (name === "money") {
            this.setState( ({
                routeState: {
                    ...this.state.routeState,
                    money: !this.state.routeState["money"], // Инвертируем значение чекбокса "money"
                    travelTime: !this.state.routeState["travelTime"] // Инвертируем значение чекбокса "time"
                }
            }));
        } else if (name === "travelTime") {
            this.setState(({
                routeState: {
                    ...this.state.routeState,
                    travelTime: !this.state.routeState["travelTime"], // Инвертируем значение чекбокса "time"
                    money: !this.state.routeState["money"]
                }
            }));
        } 
    }
}
export default App