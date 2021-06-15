#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include "canvas.hpp"


Canvas::Canvas()
    : renderTarget(QImage(100, 100, QImage::Format::Format_ARGB32)), floatingHorizon(1)
{
    renderTarget.fill(Qt::transparent);
    floatingHorizon.setStep(1);
}

/*
void drawSurface(Y_t surface, double startX, double endX, double startY, double endY, double startZ, double endZ)
{
    QPainter painter(&renderTarget);

    size_t npx = floatingHorizon.getWidth(), npz = 60;
    double npy = height() / (endY - startY);

    double dx = (endX - startX) / (npx - 1);
    double dz = (endZ - startZ) / (npz - 1);

    auto func = [=](int x, int z) -> int {
        double xf = startX + x * dx;
        double zf = startZ + z * dz;
        double yf = surface(xf, zf);

        int y = (yf - startY) / (endY - startY) * height();
        return y;
    };

    floatingHorizon.draw(painter, func, npz, 0);

    double* upperHorizont = new double[npx];
    double* lowerHorizont = new double[npx];

    for (int i = 0; i < npx; i++)
    {
        upperHorizont[i] = -1.0;
        lowerHorizont[i] = height() + 1.0;
    }
    for (int j = 0; j < npz; j++)
    {
        double zj = startZ + j * dzj;

        double ypPrev = -1.0;
        bool ypPrevVisible = false;

        double xpPrev = 0.0;
        double xpCurr = 0.0;

        double xi = startX;
        for (int i = 0; i < npx; i++)
        {
            double yi = surface(xi, 0, zj);
            double ypCurr = m * (endY - yi);
            bool yiCurrVisible = false;

            xpCurr += dxp;

            if (ypCurr > upperHorizont[i])
            {
                upperHorizont[i] = ypCurr;
                yiCurrVisible = true;
            }

            if (ypCurr < lowerHorizont[i])
            {
                lowerHorizont[i] = ypCurr;
                yiCurrVisible = true;
            }

            if (yiCurrVisible && ypPrevVisible)
                painter.drawLine(xpPrev, ypPrev, xpCurr, ypCurr);

            ypPrevVisible = yiCurrVisible;
            xpPrev = xpCurr;
            ypPrev = ypCurr;
            xi += dxi;
        }
    }
    delete[] upperHorizont;
    delete[] lowerHorizont;
}
*/

void Canvas::setSurface(Y_t function)
{
    //size_t npx = floatingHorizon.getWidth(), npz = 60;
    //double npy = height() / (end.getY() - start.getY());

    //double dx = (end.getX() - start.getX()) / (npx - 1);
    //double dz = (end.getZ() - start.getZ()) / (npz - 1);

    floatingHorizon.setSurface([=](double x, double z) {
        //double xf = start.getX() + x * dx;
        //double zf = start.getZ() + z * dz;
        double yf = function(x, z);

        return (yf - start.getY()) / (end.getY() - start.getY()) * height();
    });
}

void Canvas::setViewport(double startX, double endX, double startY, double endY, double startZ, double endZ)
{
    start = Vector(startX, startY, startZ);
    end = Vector(endX, endY, endZ);
}

void Canvas::setCounts(size_t xCount, size_t zCount)
{
    floatingHorizon.setViewport(start, end, xCount, zCount);
}

void Canvas::rotateView(double angleX, double angleY, double angleZ)
{
    floatingHorizon.rotateView(angleX, angleY, angleZ);
}

void Canvas::drawSurface()
{
    QPainter painter(&renderTarget);
    floatingHorizon.draw(painter);
}

void Canvas::clear()
{
    renderTarget.fill(Qt::transparent);
    update();
}

void Canvas::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, renderTarget);
}

void Canvas::resizeEvent(QResizeEvent* event)
{
    QSize size = event->size();

    QImage oldRenderTarget = renderTarget;
    renderTarget = QImage(size.width(), size.height(), QImage::Format::Format_ARGB32);
    renderTarget.fill(Qt::transparent);

    {
        QPainter painter(&renderTarget);
        painter.drawImage(0, 0, oldRenderTarget);
    }

    floatingHorizon.setWidth(size.width());
    update();
}
