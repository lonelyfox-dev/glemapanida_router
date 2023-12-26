import React from "react"
import City from "./City"

class Cities extends React.Component{
    render(){
        return (<div style={{ width: '100%', maxWidth: '50px'}}>
            {this.props.cities.map((el)=>(
                <City key={el.id} city={el}/>
            ))}
        </div>
            
        )
    }
}
export default Cities