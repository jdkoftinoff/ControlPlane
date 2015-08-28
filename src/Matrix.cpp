#include "ControlPlane/World.hpp"
#include "ControlPlane/Descriptor/Matrix.hpp"
#include "ControlPlane/Descriptor/MatrixSignal.hpp"

namespace ControlPlane
{
namespace Descriptor
{

Matrix::~Matrix() {}

void Matrix::storeToPDU( FixedBuffer &pdu ) const { /* TODO */ }
}
}
