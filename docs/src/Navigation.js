import { Component } from 'react';
import PropTypes from 'prop-types';

class Navigation extends Component {
  constructor(props) {
    super(props);
    this.state = {
      fullFormat: this.props.fullFormat
    };
  }

}

Navigation.PropTypes