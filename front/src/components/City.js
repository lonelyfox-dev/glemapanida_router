import React from "react"
import '../css/City.css';
import { IoCloseCircleSharp } from 'react-icons/io5'

class City extends React.Component{
    city=this.props.city
    render(){
        return (
            <div className="city">
                
                <h3>{this.city.name}</h3>
            </div>
        )
    }
}
export default City